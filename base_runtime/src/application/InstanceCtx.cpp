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

elrond::InstanceLoopCfg InstanceCtx::loopCfg()
{
    return { this->loopEnable(), this->loopInterval(), this->shared_from_this() };
}

void InstanceCtx::setup()
{
    this->_instance->instance().setup(this->shared_from_this());
}

std::future<elrond::InstanceLoopCfg> InstanceCtx::start()
{
    this->_instance->instance().start(this->shared_from_this());
    auto mode = this->loopEnable() ? std::launch::async : std::launch::deferred;
    return std::async(mode, InstanceCtx::doLoop, this->loopCfg());
}

std::future<elrond::InstanceLoopCfg> InstanceCtx::loop()
{
    return std::async(std::launch::async, InstanceCtx::doLoop, this->loopCfg());
}

void InstanceCtx::stop()
{
    this->_instance->instance().stop(this->shared_from_this());
}

elrond::InstanceLoopCfg InstanceCtx::doLoop(const elrond::InstanceLoopCfg& cfg)
{
    auto ctx = cfg.ctx;
    if (ctx->loopEnable())
    {
        if(cfg.interval > 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(cfg.interval));
        }

        ctx->_instance->instance().loop(ctx);
    }

    return ctx->loopCfg();
}

elrond::InstanceCtxP InstanceCtx::make(
    const std::string& name,
    const std::string& factory,
    const Application& app
){
    auto f = app.factory(factory);
    return std::make_shared<InstanceCtx>(InstanceCtx(f->create(name), app));
}
