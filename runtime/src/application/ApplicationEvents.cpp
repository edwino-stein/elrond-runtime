#include "application/ApplicationEvents.hpp"

using elrond::application::ApplicationEvents;
using elrond::application::State;
using IModuleInstance = elrond::interface::ModuleInstance;
using IFactoryAdapter = elrond::interface::FactoryAdapter;

ApplicationEvents::ApplicationEvents(): _state(State::CREATED) {}

State ApplicationEvents::state() const { return this->_state; }
void ApplicationEvents::state(State state)
{
    if (this->state() == state) return;
    State prev = this->_state;
    this->_state = state;
    this->onStateChanged(prev);
}

void ApplicationEvents::onStateChanged(State) const {}
void ApplicationEvents::onInstanceCreating(const std::string&, const std::string&) const {}
void ApplicationEvents::onInstanceCreated(IModuleInstance&, IFactoryAdapter&) const {}
void ApplicationEvents::onInstanceSetup(IModuleInstance&) const {}
void ApplicationEvents::onInstanceStart(IModuleInstance&) const {}
void ApplicationEvents::onInstanceStop(IModuleInstance&) const {}
