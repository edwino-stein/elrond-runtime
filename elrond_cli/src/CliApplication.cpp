#include "CliApplication.hpp"
#include "ConfigInterface.hpp"
#include "CliConsoleAdapter.hpp"

using elrond::CliApplication;
using elrond::ConfigInterface;
using elrond::CliApplication;
using elrond::application::RuntimeApplication;
using elrond::application::State;
using IModuleInstance = elrond::interface::ModuleInstance;
using IConsole = elrond::interface::Console;
using IFactoryAdapter = elrond::interface::FactoryAdapter;

CliApplication::CliApplication(
    std::shared_ptr<ConfigInterface>& config,
    CliConsoleAdapter& cliConsoleAdapter,
    elrond::application::ModuleFactoryPool& factories
):
    RuntimeApplication(factories, cliConsoleAdapter),
    config(config),
    cliConsoleAdapter(cliConsoleAdapter)
{
    this->console()->info() << "The runtime application instance created!";
} 

elrond::pointer<IConsole> CliApplication::console() const
{
    return this->cliConsoleAdapter.makeConsole("APPLICATION");
}

void CliApplication::onStateChanged(State) const
{
    switch (this->state())
    {
        case State::INITIALIZING :
            this->console()->info() << "Initializing runtime application and module instances...";
        break;

        case State::INITIALIZED :
            this->console()->info() << "The runtime application and all module instances are ready to start!";
        break;

        case State::STARTING :
            this->console()->info() << "Starting all modules instances...";
        break;

        case State::RUNNING :
            this->console()->info() << "Runtime application and all module instances are running (CTRL+C to stop)...";
        break;

        case State::RUNNING_AND_STOPPING :
            this->console()->info() << "Stopping all modules instances (CTRL+C to force stop)...";
        break;

        case State::STOPPING :
            this->console()->info() << "Waiting for all instance modules to finish...";
        break;

        case State::STOPPING_FORCED :
            this->console()->info() << "Stopping immediately all modules instances (forced stop)...";
        break;

        case State::STOPPED :
            this->console()->info() << "Runtime application and all module instances have been stopped!";
            this->console()->info() << "Bye!";
        break;
        
        default:
            break;
    }
}

void CliApplication::onInstanceCreating(const std::string& name, const std::string& factory) const
{
    this->console()->info() << "Creating module instance '" << name << "' from factory module '" << factory << "'...";
}

void CliApplication::onInstanceCreated(IModuleInstance& instance, IFactoryAdapter&) const
{
    this->console()->info() << "Module instance '" << this->name(instance) << "' was created!"; 
}

void CliApplication::onInstanceSetup(IModuleInstance& instance) const
{
    this->console()->info() << "Initializing module instance '" << this->name(instance) << "'..." ;
}

void CliApplication::onInstanceStart(IModuleInstance& instance) const
{
    this->console()->info() << "Initializing module instance '" << this->name(instance) << "'...";
}

void CliApplication::onInstanceStop(IModuleInstance& instance) const
{
    this->console()->info() << "Stopping module instance '" << this->name(instance) << "'...";
}
