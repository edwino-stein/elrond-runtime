#include "application/FactoryAdapter.hpp"

using elrond::application::FactoryAdapter;
using elrond::platform::ModuleInfo;
using elrond::interface::Factory;
using elrond::interface::Module;

FactoryAdapter::FactoryAdapter(Factory* factory)
: _factory(factory) {}

const ModuleInfo& FactoryAdapter::info() const
{ return this->_factory->info(); }

elrond::dWord FactoryAdapter::apiVersion() const
{ return this->_factory->apiVersion(); }

elrond::application::InstanceHandleP FactoryAdapter::make() const
{
    auto& f = *(this->_factory);
    return InstanceHandleP(f.create(), [&f](Module* i){ f.destroy(i); } );
}
