#include "application/ApplicationEvents.hpp"
#include "application/InstanceCtx.hpp"

using elrond::application::ApplicationEvents;
using elrond::application::State;
using elrond::application::InstanceCtx;

ApplicationEvents::ApplicationEvents(): _state(State::CREATED) {}

State ApplicationEvents::state() const
{
    return this->_state;
}

void ApplicationEvents::state(State state)
{
    if (this->state() == state) return;
    this->onStateChange(state);
    this->_state = state;
}

void ApplicationEvents::onStateChange(State) const {}
void ApplicationEvents::onInstanceCreating(const std::string&, const std::string&) const {}
void ApplicationEvents::onInstanceCreated(const InstanceCtx&) const {}
void ApplicationEvents::onInstanceSetup(const InstanceCtx&) const {}
void ApplicationEvents::onInstanceStart(const InstanceCtx&) const {}
void ApplicationEvents::onInstanceStop(const InstanceCtx&) const {}
