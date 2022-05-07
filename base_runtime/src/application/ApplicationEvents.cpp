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
