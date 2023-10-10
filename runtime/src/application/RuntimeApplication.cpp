#include "application/RuntimeApplication.hpp"

#include "application/ModuleFactoryPool.hpp"
#include "application/ModuleInstance.hpp"
#include "application/AsyncLoopTask.hpp"
#include "application/Context.hpp"
#include "interface/ConsoleAdapter.hpp"

using elrond::application::RuntimeApplication;
using elrond::application::BaseApplication;
using elrond::application::ApplicationEvents;
using elrond::application::ModuleFactoryPool;
using elrond::interface::ConsoleAdapter;
using elrond::application::AsyncLoopTask;
using elrond::application::AsyncLoopTaskP;
using elrond::application::State;
using IModuleInstance = elrond::interface::ModuleInstance;

RuntimeApplication::RuntimeApplication(ModuleFactoryPool& factories, ConsoleAdapter& consoleAdapter)
: BaseApplication(consoleAdapter), ApplicationEvents(), _factories(&factories) {}

RuntimeApplication::~RuntimeApplication() { this->stop(true); }

void RuntimeApplication::create(const std::string& name, const std::string& factory)
{
    if (this->state() != State::CREATED)
    {
        throw std::runtime_error("Invalid application state");
    }

    this->onInstanceCreating(name, factory);

    auto fact = this->_factories->get(factory);
    auto& inst = this->insert(name, *fact);

    this->onInstanceCreated(inst, *fact);
}

void RuntimeApplication::setup()
{
    if (this->state() != State::CREATED)
    {
        throw std::runtime_error("Invalid application state");
    }

    if (this->_instances.size() == 0)
    {
        throw std::runtime_error("No modules instances");
    }

    this->state(State::INITIALIZING);

    auto self = this;
    this->each(
        [&self](IModuleInstance& i, const std::string&)
        {
            self->onInstanceSetup(i);
            i.setup();
        }
    );

    this->state(State::INITIALIZED);
}

std::future<void> RuntimeApplication::start()
{
    if (this->state() != State::INITIALIZED)
    {
        throw std::runtime_error("Invalid application state");
    }

    this->state(State::STARTING);

    auto self = this;
    std::vector<AsyncLoopTaskP> tasks;
    this->each(
        [&self, &tasks](IModuleInstance& i, const std::string&)
        {
            self->onInstanceStart(i);
            i.start();

            if (i.isEnabledLoop())
            {
                auto loopTask = std::make_shared<AsyncLoopTask>(i);
                loopTask->run();
                tasks.push_back(loopTask);
            }
        }
    );

    this->state(State::RUNNING);
    return std::async(
        std::launch::async,
        RuntimeApplication::main,
        this,
        tasks
    );
}

void RuntimeApplication::stop(const bool forced)
{
    if (!this->isRunning())
    {
        return;
    }

    if (this->state() == State::RUNNING)
    {
        this->state(State::RUNNING_AND_STOPPING);
        auto self = this;
        this->each(
            [&self](IModuleInstance& i, const std::string&)
            {
                self->onInstanceStop(i);
                i.stop();
            }
        );
    }

    this->state(forced ? State::STOPPING_FORCED : State::STOPPING);
}

void RuntimeApplication::reset()
{
    switch (this->state())
    {
        case State::INITIALIZING:
        case State::STARTING:
        case State::RUNNING:
        case State::RUNNING_AND_STOPPING:
        case State::STOPPING:
        case State::STOPPING_FORCED:
            throw std::runtime_error("Invalid application state"); 
        default:
        break;
    }

    this->_instances.clear();
    this->state(State::CREATED);
}


bool RuntimeApplication::isRunning() const
{
    switch (this->state())
    {
        case State::RUNNING:
        case State::RUNNING_AND_STOPPING:
        case State::STOPPING:
            return true;
        break;
    
        default:
            return false;
        break;
    }
}

void RuntimeApplication::main(RuntimeApplication* self, std::vector<AsyncLoopTaskP> tasks)
{
    try
    {
        while (self->state() == State::RUNNING || self->state() == State::RUNNING_AND_STOPPING)
        {
            for (auto it = tasks.cbegin(); it != tasks.cend(); it++)
            {
                if(!(*it)->isRunning())
                {
                    (*it)->finish(false);
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        self->stop(true);
    }

    for (auto it = tasks.cbegin(); it != tasks.cend(); it++)
    {
        try
        {
            (*it)->finish(self->state() == State::STOPPING_FORCED);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            self->stop(true);
        }
        
    }

    self->state(State::STOPPED);
}
