#include "Signal.hpp"

std::map<enum SIG, Signal::signalCallbackT> Signal::callbacks;

Signal::Signal(){}

void Signal::attach(enum SIG sig, Signal::signalCallbackT task)
{
    Signal::detach(sig);
    Signal::callbacks[sig] = task;
    std::signal((int) sig, Signal::entryPoint);
}

void Signal::detach(enum SIG sig)
{
    if(Signal::hasAttached(sig)) Signal::callbacks.erase(sig);
    std::signal((int) sig, SIG_DFL);
}

void Signal::trigger(enum SIG sig)
{ std::raise((int) sig); }

void Signal::entryPoint(int sig)
{ if(Signal::hasAttached((enum SIG) sig)) Signal::callbacks[(enum SIG) sig](); }

bool Signal::hasAttached(enum SIG sig)
{
    std::map<enum SIG, Signal::signalCallbackT>::iterator it = Signal::callbacks.find(sig);
    return it != Signal::callbacks.end();
}
