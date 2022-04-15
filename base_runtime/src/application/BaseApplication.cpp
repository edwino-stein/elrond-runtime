#include "application/BaseApplication.hpp"

#include "application/ModuleFactoryPool.ipp"
#include "application/InstanceCtx.hpp"
#include "runtime/NullArguments.hpp"

using elrond::application::BaseApplication;
using elrond::interface::ConsoleAdapter;
using elrond::application::ModuleFactoryPool;
using elrond::application::InstanceCtx;
using elrond::runtime::Console;
using elrond::runtime::NullArguments;

BaseApplication::BaseApplication(
    elrond::interface::ConsoleAdapter& consoleAdapter,
    const elrond::application::ModuleFactoryPool& factories
):
    _consoleAdapter(consoleAdapter),
    _factories(factories)
{}

elrond::pointer<elrond::interface::Console>
BaseApplication::console(const std::string& name) const
{
    return std::make_shared<Console>(name, this->_consoleAdapter);
}

elrond::pointer<elrond::interface::Arguments>
BaseApplication::arguments(const std::string&) const
{
    return std::make_shared<NullArguments>();
}

elrond::platform::FactoryAdapterP
BaseApplication::factory(const std::string& name) const
{
    return this->_factories.get(name);
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

elrond::InstanceCtxP BaseApplication::get(const std::string& name) const
{
    try
    {
        return this->_instances.at(name);
    }
    catch(const std::out_of_range&)
    {
        throw std::runtime_error("Invalid module instance context");
    }
}

elrond::InstanceCtxP BaseApplication::add(const std::string& name, const std::string& factory)
{
    try
    {
        if (this->exists(name))
        {
            throw std::runtime_error("Module instance redefinition");
        }

        auto ctx = InstanceCtx::make(name, factory, *this);
        this->_instances[name] = ctx;
        return ctx;
    }
    catch(const std::exception& e)
    {
        throw e;
    }
}

void BaseApplication::each(elrond::InstanceCtxH handle) const
{
    for (auto it = this->_instances.cbegin(); it != this->_instances.cend(); it++)
    {
        handle(it->second);
    }
}

void BaseApplication::setup()
{
    this->each(
        [](elrond::InstanceCtxP i) { i->setup(); }
    );
}

void BaseApplication::start()
{
    this->each(
        [](elrond::InstanceCtxP i) { i->start(); }
    );

    this->each(
        [](elrond::InstanceCtxP i) { i->stop(); }
    );
}

void BaseApplication::run()
{
    this->setup();
    this->start();
}
