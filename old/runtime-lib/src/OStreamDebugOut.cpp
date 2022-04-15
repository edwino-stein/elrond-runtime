#include "OStreamDebugOut.hpp"

using elrond::runtime::OStreamDebugOut;
using elrond::interface::DebugOut;

OStreamDebugOut::OStreamDebugOut(std::ostream& os) : os(os){}

const DebugOut& OStreamDebugOut::put(const char c[]) const
{
    this->os << c;
    return *this;
}

const DebugOut& OStreamDebugOut::put(char c) const
{
    this->os << c;
    return *this;
}

const DebugOut& OStreamDebugOut::put(unsigned char c) const
{
    this->os << c;
    return *this;
}

const DebugOut& OStreamDebugOut::put(int i) const
{
    this->os << i;
    return *this;
}

const DebugOut& OStreamDebugOut::put(unsigned int i) const
{
    this->os << i;
    return *this;
}

const DebugOut& OStreamDebugOut::put(long l ) const
{
    this->os << l;
    return *this;
}

const DebugOut& OStreamDebugOut::put(unsigned long l ) const
{
    this->os << l;
    return *this;
}

const DebugOut& OStreamDebugOut::put(double d) const
{
    this->os << d;
    return *this;
}

const DebugOut& OStreamDebugOut::put(elrond::String str) const
{
    this->os << str;
    return *this;
}

const DebugOut& OStreamDebugOut::putLn(const char c[]) const
{ return this->put(c).putLn(); }

const DebugOut& OStreamDebugOut::putLn(char c) const
{ return this->put(c).putLn(); }

const DebugOut& OStreamDebugOut::putLn(unsigned char c) const
{ return this->put(c).putLn(); }

const DebugOut& OStreamDebugOut::putLn(int i) const
{ return this->put(i).putLn(); }

const DebugOut& OStreamDebugOut::putLn(unsigned int i) const
{ return this->put(i).putLn(); }

const DebugOut& OStreamDebugOut::putLn(long l) const
{ return this->put(l).putLn(); }

const DebugOut& OStreamDebugOut::putLn(unsigned long l) const
{ return this->put(l).putLn(); }

const DebugOut& OStreamDebugOut::putLn(double d) const
{ return this->put(d).putLn(); }

const DebugOut& OStreamDebugOut::putLn(elrond::String str) const
{ return this->put((std::string &) str).putLn(); }

const DebugOut& OStreamDebugOut::putLn(void) const
{
    this->os << std::endl;
    return *this;
}

const DebugOut& OStreamDebugOut::flush() const
{
    this->os << std::flush;
    return *this;
}
