#include "Signal.hpp"
#include <exception>

using elrond::Signal;

std::map<enum Signal::SIGID, Signal::SignalCallbackH> Signal::callbacks;

void Signal::attach(enum SIGID sig, SignalCallbackH handle)
{
    Signal::detach(sig);
    Signal::callbacks[sig] = handle;
    std::signal(static_cast<int>(sig), Signal::entryPoint);
}

void Signal::detach(enum SIGID sig)
{
    if(Signal::hasAttached(sig)) Signal::callbacks.erase(sig);
    std::signal(static_cast<int>(sig), SIG_DFL);
}

bool Signal::hasAttached(enum SIGID sig)
{
    try
    {
        Signal::callbacks.at(sig);
        return true;
    }
    catch(const std::exception&)
    {
        return false;
    }
}

void Signal::trigger(enum SIGID sig) { std::raise(static_cast<int>(sig)); }

void Signal::entryPoint(int sig)
{
    auto sigid = static_cast<enum SIGID>(sig);
    if (Signal::hasAttached(sigid))
    {
        Signal::callbacks[sigid](sigid); 
    }
}

