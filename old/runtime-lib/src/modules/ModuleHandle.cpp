#include "modules/ModuleHandle.hpp"
#include "modules/ModuleFactory.hpp"

using elrond::runtime::ModuleHandle;
using elrond::interface::Module;
using elrond::runtime::ModuleFactoryP;

ModuleHandle::ModuleHandle(elrond::String name, ModuleFactoryP factory):
    _name(name),
    _factory(factory),
    name(_name),
    instance(_instance),
    factory(_factory)
{ this->_instance = this->factory->newInstance(this->_name); }

ModuleHandle::~ModuleHandle()
{
    if(this->_instance != nullptr) this->factory->deleteInstance(this->_instance);
    this->_instance = nullptr;
}

void ModuleHandle::entryPoint(ModuleHandle* const mh)
{
    while (mh->running){
        if(mh->started){
            mh->instance->loop();
            elrond::delay(mh->lc.interval);
        }
    }
}

void ModuleHandle::asyncRun()
{
    if(this->running) return;
    this->running = true;
    this->thread = std::thread(ModuleHandle::entryPoint, this);
}

void ModuleHandle::asyncStop(const bool join)
{
    if(!this->running) return;
    this->running = false;
    elrond::delay(this->lc.interval);
    if(join && this->thread.joinable()) this->thread.join();
    else this->thread.detach();
}

void ModuleHandle::syncLoop()
{
    if(!this->started) return;
    if(this->timout > elrond::millis()) return;
    this->instance->loop();
    this->timout = elrond::millis() + this->lc.interval;
}
