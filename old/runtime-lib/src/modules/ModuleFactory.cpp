#include "modules/ModuleFactory.hpp"

using elrond::runtime::ModuleFactory;
using elrond::runtime::ModuleInfo;
using elrond::interface::Module;

ModuleFactory::ModuleFactory(elrond::String name):
_name(name), info(_info){}

ModuleFactory::~ModuleFactory(){}

bool ModuleFactory::match(elrond::String const& name) const
{
    return this->_name == name;
}

elrond::String ModuleInfo::about() const
{
    std::ostringstream oss;

    oss << this->name();

    if(this->version != "") oss << " v" << this->version;

    oss << " (API " << std::to_string(this->majorVer())
        << "." << std::to_string(this->minorVer())
        << "." << std::to_string(this->pathVer()) << ")";

    if(this->authorName != ""){
        oss << " by " << this->authorName;
        if(this->authorEmail != "") oss << " <" << this->authorEmail << ">";
    }

    return oss.str();
}

elrond::String ModuleInfo::name() const
{ return this->prettyName != "" ? this->prettyName : this->mainClass; }

unsigned int ModuleInfo::majorVer() const
{ return elrond::lowByte(elrond::highWord(this->apiVer)); }

unsigned int ModuleInfo::minorVer() const
{ return elrond::highByte(elrond::lowWord(this->apiVer)); }

unsigned int ModuleInfo::pathVer() const
{ return elrond::lowByte(elrond::lowWord(this->apiVer)); }
