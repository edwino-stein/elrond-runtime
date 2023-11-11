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
{} 

elrond::pointer<IConsole> CliApplication::console() const
{
    return this->cliConsoleAdapter.makeConsole("APPLICATION");
}
