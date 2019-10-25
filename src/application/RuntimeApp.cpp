#include "application/RuntimeApp.hpp"

#include <fstream>

#include "exceptions/Exception.hpp"
#include "modules/ModuleHandle.hpp"
#include "channel/ChannelManager.hpp"

using elrond::channel::BaseChannelManager;

RuntimeApp::RuntimeApp() : LoaderRuntimeApp(){
    this->info.mainClass = "RuntimeApp";
    this->info.apiVer = ELROND_API_VERSION;
    this->info.apiRevision = ELROND_API_REVISION;
    this->info.prettyName = "Elrond Runtime for Linux";
    this->info.authorName = "Edwino Stein";
    this->info.authorEmail = "edwino.stein@gmail.com";
    this->info.version = std::to_string(ELROND_API_VERSION) + "." + std::to_string(ELROND_API_REVISION) + "-alpha";
}

RuntimeApp::~RuntimeApp(){}

void RuntimeApp::run(){

    std::for_each(
        this->chmgrs.begin(),
        this->chmgrs.end(),
        [](ChannelManagerP chm){ chm->run(); }
    );

    this->startModules();

    std::cout << " * Application running (CTRL+C to stop)..." << '\n';
    while(this->loop){}
}

void RuntimeApp::stop(bool force){

    if(force) this->loop = true;
    if(!this->loop) return;

    this->stopModules();
    this->loop = false;

    std::for_each(
        this->chmgrs.begin(),
        this->chmgrs.end(),
        [&force](ChannelManagerP chm){ chm->stop(!force); }
    );

    std::for_each(
        this->modules.begin(),
        this->modules.end(),
        [&force](ModuleHandleP mh){ mh->asyncStop(!force); }
    );
}

void RuntimeApp::init(int argc, char const *argv[]){

    std::cout << " * Starting " << this->info.about() << "..." << std::endl;

    if(argc <= 1) throw Exception("Missing JSON config file");

    Json cfg;
    RuntimeApp::readJsonFromFile(argv[1], cfg);

    if(!cfg["modules"].is_object()) throw Exception("JSON error", Exception("Missing \"modules\" JSON object"));
    if(!cfg["init"].is_object()) throw Exception("JSON error", Exception("Missing \"init\" JSON object"));

    this->parseModules(cfg["modules"]);
    this->parseOptions(cfg["options"]);
    this->initModules(cfg["init"]);
}

void RuntimeApp::parseOptions(Json &cfg){
    if(cfg.is_object()){
        this->parseChmgrs(cfg["chmgrs"]);
    }
}

void RuntimeApp::parseChmgrs(Json &cfg){

    if(cfg.is_array()){

        std::cout << " * Initializing channel managers (" << cfg.size() << ")..." << std::endl;

        for(auto& el : cfg.items()){

            if(!el.value().is_object()) continue;
            Json &chmCfg = el.value();

            ModuleHandleP mod = this->findModule(chmCfg["transport"]);
            if(mod == nullptr){
                std::cout << "  WARNING: The \"" << String(chmCfg["transport"]) << "\" instance not found.";
                std::cout << "  Ignoring this channel manager!" << std::endl;
                continue;
            }

            if(mod->module->getType() != elrond::ModuleType::TRANSPORT){
                std::cout << "  WARNING: The \"" << String(chmCfg["transport"]) << "\" instance is not a transport module!.";
                std::cout << "  Ignoring this channel manager!" << std::endl;
                continue;
            }

            unsigned int timout = 0;
            if(chmCfg["timeout"].is_number_integer()) timout = chmCfg["timeout"].get<int>();

            ChannelManagerP chmgr = std::make_shared<ChannelManager>(
                (elrond::modules::BaseTransportModule &) *(mod->module),
                (elrond::sizeT) chmCfg["tx"].get<int>(),
                (elrond::sizeT) chmCfg["rx"].get<int>(),
                timout
            );

            chmgr->init();
            this->chmgrs.push_back(chmgr);
        }
    }
    else std::cout << "  WARNING: Missing \"chmgrs\" key in JSON file!" << std::endl;

    if(this->chmgrs.size() <= 0) std::cout << "  WARNING: No channel managers defined!" << std::endl;
}

BaseChannelManager &RuntimeApp::getChannelManager(const elrond::sizeT id) const {

    if(id >= this->chmgrs.size()){
        throw Exception(
            "Invalid channel manager",
            Exception("The channel manager with index " + std::to_string(id) + " has not defined")
        );
    }

    return *this->chmgrs[id];
}

void RuntimeApp::readJsonFromFile(String file, Json &json){

    try{

        std::ifstream ifs(file);
        if(!ifs.good()) throw Exception("\"" + file + "\": No such file or directory");

        try{
            ifs >> json;
        }
        catch(std::exception &e){
            throw Exception(e);
        }
    }
    catch(Exception &e){
        throw Exception("Unable to read config file", e);
    }
}
