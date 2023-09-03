#include "application/BaseApplication.hpp"

#include "application/ModuleInstance.hpp"
#include "interface/ConsoleAdapter.hpp"

using elrond::application::BaseApplication;
using elrond::application::ModuleInstanceP;
using elrond::application::FactoryP;
using elrond::interface::Arguments;
using elrond::runtime::NullArguments;
using elrond::runtime::Console;
using elrond::interface::ConsoleAdapter;

using IModuleInstance = elrond::interface::ModuleInstance;
using IFactoryAdapter = elrond::interface::FactoryAdapter;
using IConsole = elrond::interface::Console;

BaseApplication::BaseApplication(ConsoleAdapter& consoleAdapter) : _instances(), _consoleAdapter(&consoleAdapter) {}

std::string BaseApplication::name(IModuleInstance& inst) const
{
    for (auto it = this->_instances.cbegin(); it != this->_instances.cend(); it++)
    {
        if (it->second.get() == &inst)
        {
            return it->first;
        }
    }

    throw std::runtime_error("Unable to find module instance name");
}

elrond::pointer<IConsole> BaseApplication::console(IModuleInstance& inst) const
{
    std::string name = this->name(inst);
    return std::make_shared<Console>(this->_consoleAdapter->makeConsoleStreamAdapter(name));
}

elrond::pointer<Arguments> BaseApplication::arguments(IModuleInstance&) const
{ return std::make_shared<NullArguments>(); }

IModuleInstance& BaseApplication::get(const std::string& name) const
{
    try
    {
        auto inst = this->_instances.at(name);
        return *inst;
    }
    catch(const std::out_of_range&)
    {
        throw std::runtime_error("Invalid module instance");
    }
}

bool BaseApplication::exists(const std::string& name) const
{
    try
    {
        this->get(name);
        return true;
    }
    catch(const std::exception&)
    {
        return false;
    }
}

IModuleInstance& BaseApplication::insert(const std::string& name, IFactoryAdapter& factory)
{
    if (this->exists(name))
    {
        throw std::runtime_error("Module instance redefinition");
    }

    auto inst = std::make_shared<ModuleInstance>(*this, factory);
    this->_instances[name] = inst;
    return *inst;
}

void BaseApplication::each(elrond::procedure<IModuleInstance&, const std::string&> handle) const
{
    for (auto it = this->_instances.cbegin(); it != this->_instances.cend(); it++)
    {
        handle(*(it->second), it->first);
    }
}
