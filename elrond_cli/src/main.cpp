#include "ProgArguments.hpp"
#include "CliConsoleAdapter.hpp"
#include "JsonConfig.hpp"
#include "CliApplication.hpp"
#include "Signal.hpp"

using elrond::ProgArguments;
using elrond::CliConsoleAdapter;
using elrond::JsonConfig;
using elrond::CliApplication;
using elrond::Signal;
using elrond::platform::ModuleInfo;
using elrond::application::ModuleFactoryPool;
using elrond::application::State;

const ModuleInfo cliAppVersion {"Elrond CLI Application", "Edwino Stein", "edwino.stein@gmail.com", "0.5.0-alpha"};

std::string printModuleInfo(const ModuleInfo& info, elrond::dWord apiVersion = elrond::getApiVersion())
{
    std::ostringstream oss;

    oss << info.name << ' ';

    if (info.version != "")
    {
        if (info.version[0] != 'v' && info.version[0] != 'V') oss << 'v';
        oss << info.version << ' ';
    }

    oss << "(API v"
        << std::to_string(elrond::lowByteHighWord(apiVersion)) << '.'
        << std::to_string(elrond::highByteLowWord(apiVersion)) << '.'
        << std::to_string(elrond::lowByteLowWord(apiVersion));

    if ((elrond::highByteHighWord(apiVersion) & 0x0F) == 0x02) oss << "-alpha";
    if ((elrond::highByteHighWord(apiVersion) & 0x0F) == 0x01) oss << "-beta";
    if ((elrond::highByteHighWord(apiVersion) & 0xF0) == 0xF0) oss << " [D]";
    oss << ')';

    if (info.author != "")
    {
        oss << " by " << info.author;
        if (info.email != "")
        {
            oss << " <" << info.email << ">";
        }
    }

    return oss.str();
}

std::unique_ptr<CliApplication> loadApp(
    CliConsoleAdapter& cliConsoleAdapter,
    ModuleFactoryPool& factories,
    std::shared_ptr<elrond::ConfigInterface> config
){
    auto console = cliConsoleAdapter.makeConsole("BOOTSTRAP");

    if (config->totalFactories() > 0)
    {
        console->info() << "Loading external module factories (" << config->totalFactories() << ")...";
        config->eachFactory(
            [&factories, &console](const std::string& name, const std::string& path)
            {
                console->info() << "Loading external module factory '" << name << "' from '" << path << "'...";
                auto f = factories.load(name, path);
                console->info() << "Loaded external module factory '" << name << "': " << printModuleInfo(f->info(), f->apiVersion());
            }
        );
    }
    else
    {
        console->info() << "No external module factories defined to load!";
    }

    std::unique_ptr<CliApplication> app(new CliApplication(config, cliConsoleAdapter, factories));

    console->info() << "Creating module instances (" << config->totalInstances() << ")...";
    config->eachInstanceFactory( [&app](const std::string& name, const std::string& factory){ app->create(name, factory); });

    console->info() << "The runtime application instance was successfully loaded!";

    return app;
}

int main(int argc, char* argv[])
{
    CliConsoleAdapter cliConsoleAdapter;
    ModuleFactoryPool factories(ModuleFactoryPool::internals());
    std::unique_ptr<CliApplication> app(nullptr);

    try
    {
        auto args = ProgArguments::parse(argc, argv);
        auto console = cliConsoleAdapter.makeConsole("ENTRYPOINT");
        console->info() << "Initializing " << printModuleInfo(cliAppVersion) << "...";
        
        console->info() << "Reading configuration file '" << args->configFile << "'...";
        auto config = JsonConfig::fromFile(args->configFile);
        
        if (args->dumpConfig)
        {
            console->info() << "Validated configuration: " << config->dumpConfig();
        }

        app = loadApp(cliConsoleAdapter, factories, config);
    }
    catch(const std::invalid_argument&)
    {
        return 1;
    }
    catch(const std::exception& e)
    {
        cliConsoleAdapter.makeConsole("ENTRYPOINT")->error() << e.what();
        return 1;
    }

    try
    {
        app->setup();

        Signal::attach(
            Signal::INT,
            [&app, &cliConsoleAdapter](enum Signal::SIGID sig)
            {
                std::cout << "\b\b";
                cliConsoleAdapter.makeConsole("ENTRYPOINT")->info() << "Received Signal INT (" << static_cast<int>(sig) << ")";
                app->stop(app->state() == State::STOPPING);
            }
        );

        auto f = app->start();
        f.wait();
        f.get();
    }
    catch(const std::exception& e)
    {
        cliConsoleAdapter.makeConsole("ENTRYPOINT")->error() << e.what();
        return 1;
    }

    return 0;
}
