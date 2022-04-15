#include "application/InstanceCtx.hpp"
#include "interface/Application.hpp"

using elrond::application::InstanceCtx;
using elrond::interface::Application;
using elrond::platform::ModuleInstanceP;
using elrond::interface::Console;
using elrond::interface::Arguments;

InstanceCtx::InstanceCtx(ModuleInstanceP inst, const Application& app)
:
    _instance(inst),
    _app(app),
    _loopEnabled(false),
    _loopAsync(false),
    _loopInterval(0)
{}

elrond::pointer<Console> InstanceCtx::console() const
{
    return this->_app.console(this->name());
}

elrond::pointer<Arguments> InstanceCtx::arguments() const
{
    return this->_app.arguments(this->name());
}

elrond::string InstanceCtx::name() const
{
    return this->_instance->name();
}

void InstanceCtx::loopEnable(bool enable)
{
    this->_loopEnabled = enable;
}

void InstanceCtx::loopInterval(elrond::timeT interval)
{
    this->_loopInterval = interval;
}

void InstanceCtx::loopAsync(bool enable)
{
    this->_loopAsync = enable;
}

bool InstanceCtx::loopEnable() const
{
    return this->_loopEnabled;
}

elrond::timeT InstanceCtx::loopInterval() const
{
    return this->_loopInterval;
}

bool InstanceCtx::loopAsync() const
{
    return this->_loopAsync;
}

void InstanceCtx::setup()
{
    this->_instance->instance().setup(this->shared_from_this());
}

void InstanceCtx::start()
{
    this->_instance->instance().start(this->shared_from_this());
}

void InstanceCtx::loop()
{
    this->_instance->instance().loop(this->shared_from_this());
}

void InstanceCtx::stop()
{
    this->_instance->instance().stop(this->shared_from_this());
}

elrond::InstanceCtxP InstanceCtx::make(
    const std::string& name,
    const std::string& factory,
    const Application& app
){
    auto f = app.factory(factory);
    return std::make_shared<InstanceCtx>(InstanceCtx(f->create(name), app));
}
