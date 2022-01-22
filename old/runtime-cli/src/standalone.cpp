#include "standalone.hpp"
#include "Signal.hpp"
#include "Stacktrace.hpp"
#include "JsonConfigMap.hpp"

#include "modules/DlModuleFactory.hpp"
#include "modules/VirtualGpio.hpp"
#include "modules/Serial.hpp"
#include "modules/Udp.hpp"

#include <fstream>

using elrond::runtime::RuntimeApp;
using elrond::runtime::OStreamDebugOut;
using elrond::runtime::ChannelManager;
using elrond::runtime::ChannelManagerP;
using elrond::runtime::ModuleFactoryP;
using elrond::runtime::ModulesFactoriesV;
using elrond::runtime::InternalModuleFactory;
using elrond::runtime::ModuleInfo;
using elrond::runtime::Exception;

elrond::runtime::ModuleInfo appInfo = {
    .mainClass = "RuntimeApp",
    .apiVer = ELROND_API_VERSION,
    .prettyName = "Elrond Runtime for Linux",
    .authorName = "Edwino Stein",
    .authorEmail = "edwino.stein@gmail.com",
    .version = ELROND_API_VERSION_STR
};

int run(int argc, char const* argv[])
{
    OStreamDebugOut dout(std::cout);
    RuntimeApp app(dout);
    bool loop = false;

    Signal::attach(
        SIG::INT,
        [&app, &loop]()
        {
            std::cout << "\b\b * Received Signal INT (" << (int) SIG::INT << "): ";
            std::cout << "Interrupt" << std::endl;
            loop = false;
            stopApplication(app, 0);
        }
    );

    Signal::attach(
        SIG::TERM,
        [&app, &loop]()
        {
            std::cout << "\b\b * Received Signal TERM (" << (int) SIG::TERM << "): ";
            std::cout << "Terminate" << std::endl;
            loop = false;
            stopApplication(app, 128 + (int) SIG::TERM);
        }
    );

    Signal::attach(
        SIG::SEGV,
        [&app, &loop]()
        {
            std::cout << std::endl << " * Received Signal SEGV (" << (int) SIG::SEGV << "): ";
            std::cout << "Segmentation fault" << std::endl << std::endl;

            Stacktrace::dump(std::cout, 4);
            std::cout << std::endl;

            loop = false;
            stopApplication(app, 128 + (int) SIG::SEGV);
        }
    );

    try{
        std::cout << " * Starting " << appInfo.about() << "..." << std::endl;
        loadApplication(app, argc, argv);

        std::cout << " * Starting all modules instances..." << std::endl;
        app.start();

        std::cout << " * Application running (CTRL+C to stop)..." << '\n';
        loop = true;
        app.loop([&loop](){ return loop; });
    }
    catch(Exception &e){
        std::cout << std::endl << " * An error occurred" << std::endl;
        e.what(std::cout);
        std::cout << std::endl;
    }

    app.stop(false);
    return 0;
}

void loadApplication(RuntimeApp& app, const int argc, char const* argv[])
{
    if(argc <= 1) throw Exception("Missing JSON config file");

    Json cfg;
    readJsonFromFile(argv[1], cfg);

    if(!cfg["modules"].is_object())
        throw Exception("JSON error", Exception("Missing \"modules\" JSON object"));

    if(!cfg["init"].is_object())
        throw Exception("JSON error", Exception("Missing \"init\" JSON object"));

    parseModules(app, cfg["modules"]);
    parseChmgrs(app, cfg["options"]["chmgrs"]);
    initModules(app, cfg);
}

void stopApplication(RuntimeApp& app, const int code)
{
    std::cout << " * Stopping all modules instances..." << std::endl;
    app.stop(code > 0);
    if(code > 0) std::exit(code);
}

void parseModules(RuntimeApp& app, Json& cfg)
{
    std::cout << " * Creating instance modules (" << cfg.size() << ")..." << std::endl;

    ModulesFactoriesV factories = newStandaloneModulesFactorories();

    elrond::sizeT i = 1;
    for (auto& el : cfg.items())
    {
        ModuleInfo info;
        elrond::String name(el.key());
        elrond::String type(el.value());

        std::cout << "\t#" << i++ << " Define instance \"" << name;
        std::cout << "\" from \"" << type << "\"" << std::endl;

        try{
            info = app.defineModule(name, type, factories);
        }
        catch(Exception &e){
            ModuleFactoryP f = std::make_shared<DlModuleFactory>(type, app);
            factories.push_back(f);
            info = app.defineModule(name, type, factories);
        }

        std::cout << "\t   Created instance \"" << name;
        std::cout << "\" of " << info.about() << std::endl;

    }
}

void initModules(RuntimeApp& app, Json& cfg)
{
    Json& modules = cfg["modules"];
    Json& init = cfg["init"];

    std::cout << " * Initializing modules instances (" << modules.size() << ")..." << std::endl;

    elrond::sizeT i = 1;
    for (auto& el : modules.items()){

        elrond::String name(el.key());

        std::cout << "\t#" << i++ << ": Initializing instance \"" <<  name << "\"..." << std::endl;

        Json &jc = init[name];
        if(!jc.is_object()) continue;

        JsonConfigMap jcm(jc);
        app.initModule(name, jcm);
    }
}

void parseChmgrs(RuntimeApp& app, Json &cfg)
{
    elrond::sizeT i = 0;

    if(cfg.is_array())
    {
        std::cout << " * Initializing channel managers (" << cfg.size() << ")..." << std::endl;

        for(auto& el : cfg.items())
        {
            if(!el.value().is_object()) continue;

            Json &chmCfg = el.value();
            elrond::String transport = chmCfg["transport"];
            ChannelManagerP chmgr = nullptr;
            elrond::sizeT tx = chmCfg["tx"].get<int>();
            elrond::sizeT rx = chmCfg["rx"].get<int>();
            elrond::sizeT fps = 0;

            if(chmCfg["tx-fps"].is_number_integer())
                fps = chmCfg["tx-fps"].get<int>();

            std::cout << "\t#" << (i + 1) << " Define channel manager using instance \"";
            std::cout << transport << "\"" << std::endl;

            try{
                chmgr = app.defineChannelManager(transport, tx, rx, fps);
            }
            catch(Exception &e){
                std::cout << "\t    WARNING: " + e.message << ". Ignoring this channel manager!" << std::endl;
                continue;
            }

            std::cout << "\t   TX Channels: " << tx;
            std::cout << " [" << chmgr->getTxBufferSize() << " Bytes";
            std::cout << (fps == 0 ? "" : "; " + std::to_string(fps) + " fps");
            std::cout << (fps == 0 ? "]" : "; " + std::to_string(chmgr->getTxBufferSize() * fps) + " Bps]") << std::endl;
            std::cout << "\t   RX Channels: " << rx << " [" << chmgr->getRxBufferSize() << " Bytes]" << std::endl;

            ++i;
        }
    }
    else std::cout << " * WARNING: Missing \"chmgrs\" key in JSON file!" << std::endl;

    if(i == 0) std::cout << " * WARNING: No channel managers defined!" << std::endl;
}

void readJsonFromFile(elrond::String file, Json& json)
{
    try{
        std::ifstream ifs(file);
        if(!ifs.good()) throw Exception("\"" + file + "\": No such file or directory");

        try{ ifs >> json; }
        catch(std::exception &e){ throw Exception(e); }
    }
    catch(Exception &e){
        throw Exception("Unable to read config file", e);
    }
}

ModulesFactoriesV newStandaloneModulesFactorories()
{
    ModulesFactoriesV factories = RuntimeApp::newModulesFactories();
    factories.push_back(std::make_shared<InternalModuleFactory<elrond::runtime::VirtualGpio>>());
    factories.push_back(std::make_shared<InternalModuleFactory<elrond::runtime::Serial>>());
    factories.push_back(std::make_shared<InternalModuleFactory<elrond::runtime::Udp>>());
    return std::move(factories);
}
