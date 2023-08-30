#include "application/Context.hpp"
#include "interface/Application.hpp"
#include "interface/ModuleInstance.hpp"

using elrond::application::Context;
using elrond::interface::Application;
using elrond::interface::Arguments;
using elrond::interface::Console;
using IModuleInstance = elrond::interface::ModuleInstance;

Context::Context(IModuleInstance& instance) : _instance(instance) {}

elrond::string Context::name() const
{ return this->_instance.app().name(this->_instance); }

elrond::pointer<Console> Context::console() const
{ return this->_instance.app().console(this->_instance); }

elrond::pointer<Arguments> Context::arguments() const
{ return this->_instance.app().arguments(this->_instance); }

void Context::setLoopEvery(const elrond::TimeSpan& ts)
{
    this->_instance.enableLoop(true);
    this->_instance.loopTs(ts);
}

void Context::unsetLoop()
{
    this->_instance.enableLoop(false);
    this->_instance.loopTs(elrond::seconds(0));
}
