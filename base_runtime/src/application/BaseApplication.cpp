#include "application/BaseApplication.hpp"

#include "application/ModuleFactoryPool.ipp"
#include "application/InstanceCtx.hpp"
#include "runtime/NullArguments.hpp"
#include "runtime/FutureHolder.ipp"

using elrond::application::BaseApplication;
using elrond::application::ApplicationEvents;
using elrond::application::State;
using elrond::interface::ConsoleAdapter;
using elrond::application::ModuleFactoryPool;
using elrond::application::InstanceCtx;
using elrond::runtime::Console;
using elrond::runtime::NullArguments;
using elrond::runtime::FutureHolder;


BaseApplication::BaseApplication(
    elrond::interface::ConsoleAdapter& consoleAdapter,
    const elrond::application::ModuleFactoryPool& factories
):
    _consoleAdapter(consoleAdapter),
    _factories(factories),
    _instances()
{}

BaseApplication::~BaseApplication()
{
    if (this->state() == State::RUNNING) this->stop();
}

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
    if (this->state() != State::CREATED)
    {
        throw std::runtime_error("Invalid application state");
    }

    this->onInstanceCreating(name, factory);

    if (this->exists(name))
    {
        throw std::runtime_error("Module instance redefinition");
    }

    auto ctx = InstanceCtx::make(name, factory, *this);
    this->_instances[name] = ctx;
    this->onInstanceCreated(*ctx);

    return ctx;
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
    if (this->state() != State::CREATED) return;
    
    this->state(State::INITIALIZING);

    auto self = this;
    this->each(
        [&self](elrond::InstanceCtxP i)
        {
            self->onInstanceSetup(*i);
            i->setup();
        }
    );

    this->state(State::INITIALIZED);
}

std::future<void> BaseApplication::start()
{
    if (this->state() != State::INITIALIZED)
    {
        throw std::runtime_error("Invalid application state");
    }

    this->state(State::STARTING);

    auto self = this;
    std::queue<elrond::FutureHolderP<elrond::InstanceLoopCfg>> loops;
    this->each(
        [&self, &loops](elrond::InstanceCtxP i)
        {
            self->onInstanceStart(*i);
            loops.push(
                FutureHolder<elrond::InstanceLoopCfg>::make(i->start())
            );
        }
    );

    this->state(State::RUNNING);
    return std::async(
        BaseApplication::mainLoop,
        std::ref(*this),
        loops
    );
}

void BaseApplication::stop(const bool forced)
{
    const auto state = this->state();
    if (!forced && state != State::RUNNING) return;

    this->state(State::STOPPING);

    if (state == State::RUNNING)
    {
        auto self = this;
        this->each(
            [&self](elrond::InstanceCtxP i)
            {
                self->onInstanceStop(*i);
                i->stop();
            }
        );
    }
}

std::future<void> BaseApplication::run()
{
    this->setup();
    return this->start();
}

void BaseApplication::reset()
{
    switch (this->state())
    {
        case State::INITIALIZING:
        case State::STARTING:
        case State::RUNNING:
        case State::STOPPING:
            throw std::runtime_error("Invalid application state"); 
        default:
        break;
    }

    this->_instances.clear();
    this->state(State::CREATED);
}

void BaseApplication::mainLoop(
    BaseApplication& app,
    std::queue<elrond::FutureHolderP<elrond::InstanceLoopCfg>> loops
){
    while (app.state() == State::RUNNING)
    {
        const elrond::sizeT len = loops.size();
        for (elrond::sizeT i = 0; i < len; ++i)
        {
            if (app.state() == State::STOPPING) break;

            auto l = loops.front();
            loops.pop();

            if(!l->ready())
            {
                loops.push(l);
                continue;
            }

            auto cfg = l->get();
            if (!cfg.enabled) continue;

            loops.push(
                FutureHolder<elrond::InstanceLoopCfg>::make(cfg.ctx->loop())
            );
        }
    }

    while(loops.size() > 0)
    {
        auto l = loops.front();
        loops.pop();

        if(l->ready()) l->get();
        else loops.push(l);
    }

    app.state(State::STOPPED);
}
