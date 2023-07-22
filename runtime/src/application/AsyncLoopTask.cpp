#include "application/AsyncLoopTask.hpp"
#include "interface/LoopTask.hpp"

using elrond::application::AsyncLoopTask;
using elrond::interface::LoopTask;

AsyncLoopTask::AsyncLoopTask(LoopTask& task)
:
    _task(&task),
    _status(STATUS::NOT_STARTED),
    _thread(),
    _future(nullptr),
    _detached(false)
{}

AsyncLoopTask::~AsyncLoopTask()
{
    try
    {
        this->finish(true);
    }
    catch(...){}
}

void AsyncLoopTask::run()
{
    if (this->_status != STATUS::NOT_STARTED)
    {
        throw std::runtime_error("Task already started");
    }

    this->_status = STATUS::STARING;
    std::promise<void> promise;
    this->_future.reset(new std::future<void>(promise.get_future()));
    this->_thread = std::thread(AsyncLoopTask::loop, this, std::move(promise));
}

void AsyncLoopTask::stop()
{
    if (this->isRunning())
    {
        this->_status = STATUS::STOPPING;
    }
}

bool AsyncLoopTask::isRunning() const
{
    return this->_status == STATUS::RUNNING || this->_status == STATUS::SLEEPING;
}

bool AsyncLoopTask::isStopping() const
{
    return this->_status == STATUS::STOPPING;
}

bool AsyncLoopTask::wasDetached() const
{
    return this->_detached;
}

void AsyncLoopTask::finish(bool force)
{
    if (this->_status == STATUS::FINISHED || this->_status == STATUS::NOT_STARTED)
    {
        return;
    }

    if (this->isRunning() || this->isStopping())
    {
        this->stop();

        if (force)
        {
            this->_thread.detach();
            this->_status = STATUS::FINISHED;
            this->_detached = true;
            return;
        }
        else
        {
            while (this->isStopping());
        }
    }

    this->_future->wait();
    if (this->_thread.joinable()) 
    {
        this->_thread.join();
    }

    this->_status = STATUS::FINISHED;
    this->_future->get();
}

void AsyncLoopTask::loop(AsyncLoopTask* self, std::promise<void> promise)
{
    STATUS status = STATUS::STARING;
    self->_status.compare_exchange_strong(status, STATUS::RUNNING);
    const auto delay = std::chrono::microseconds(elrond::microseconds(self->_task->timespan()).count);

    try
    {
        while (self->_status == STATUS::RUNNING)
        {
            self->_task->loop();
    
            if (delay.count() > 0)
            {
                status = STATUS::RUNNING;
                if (self->_status.compare_exchange_strong(status, STATUS::SLEEPING))
                {
                    std::this_thread::sleep_for(delay);
                    status = STATUS::SLEEPING;
                    self->_status.compare_exchange_strong(status, STATUS::RUNNING);
                }
            }
        }

        promise.set_value_at_thread_exit();
    }
    catch(...)
    {
        promise.set_exception_at_thread_exit(std::current_exception());
    }

    status = STATUS::RUNNING;
    self->_status.compare_exchange_strong(status, STATUS::STOPPED);

    status = STATUS::SLEEPING;
    self->_status.compare_exchange_strong(status, STATUS::STOPPED);

    status = STATUS::STOPPING;
    self->_status.compare_exchange_strong(status, STATUS::STOPPED);
}
