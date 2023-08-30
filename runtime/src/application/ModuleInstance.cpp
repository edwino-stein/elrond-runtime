#include "application/ModuleInstance.hpp"
#include "interface/Application.hpp"
#include "interface/FactoryAdapter.hpp"
#include "application/Context.hpp"

using elrond::application::ModuleInstance;
using elrond::interface::Application;
using elrond::application::FactoryP;
using elrond::application::Context;

ModuleInstance::ModuleInstance(Application& app, FactoryP factory)
: _app(&app), _instance(factory->make()), _enableLoop(false), _loopTs(elrond::seconds(0)) {}

void ModuleInstance::setup()
{ this->_instance->setup(std::make_shared<Context>(*this)); }

void ModuleInstance::start()
{ this->_instance->start(std::make_shared<Context>(*this)); }

void ModuleInstance::loop()
{ this->_instance->loop(std::make_shared<Context>(*this)); }

void ModuleInstance::stop()
{ this->_instance->stop(std::make_shared<Context>(*this)); }

Application& ModuleInstance::app() const
{ return *(this->_app); }

bool ModuleInstance::isEnabledLoop() const
{ return this->_enableLoop; }

void ModuleInstance::enableLoop(bool enable)
{ this->_enableLoop = enable; }

void ModuleInstance::loopTs(const elrond::TimeSpan& ts)
{ this->_loopTs = ts; }

elrond::TimeSpan ModuleInstance::timespan() const
{ return this->_loopTs; }
