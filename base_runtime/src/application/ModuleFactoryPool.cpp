#include "application/ModuleFactoryPool.hpp"
#include "platform/ExternalFactoryAdapter.hpp"

#include "module/HelloWorld.hpp"

using elrond::application::ModuleFactoryPool;
using elrond::platform::FactoriesM;
using elrond::platform::FactoryAdapterP;
using elrond::platform::ExternalFactoryAdapter;

ModuleFactoryPool ModuleFactoryPool::createWithCommonInternals()
{
    return FactoriesM {
        {
            ELROND_DEF_INTERNAL_MOD_FACTORY(
                "elrond::HelloWorld",
                elrond::module::HelloWorld,
                "Hello World",
                "Edwino Stein",
                "edwino.stein@gmail.com",
                "1.0.0"
            )
        }
    };
}

ModuleFactoryPool::ModuleFactoryPool(const FactoriesM& factories)
: _factories(factories) {}

FactoryAdapterP ModuleFactoryPool::get(const std::string& name) const
{
    try
    {
        return this->_factories.at(name);
    }
    catch(const std::out_of_range&)
    {
        throw std::runtime_error("Factory not found");
    }
}

ModuleFactoryPool& ModuleFactoryPool::copyFrom(const ModuleFactoryPool& other)
{
    return this->copyFrom(other._factories, false);
}

ModuleFactoryPool& ModuleFactoryPool::copyFrom(const ModuleFactoryPool& other, bool override)
{
    return this->copyFrom(other._factories, override);
}

ModuleFactoryPool& ModuleFactoryPool::copyFrom(const FactoriesM& factories)
{
    return this->copyFrom(factories, false);
}

ModuleFactoryPool& ModuleFactoryPool::copyFrom(const FactoriesM& factories, bool override)
{
    for (auto it = factories.cbegin(); it != factories.cend(); it++)
    {
        if(this->_factories.find(it->first) == this->_factories.end() || override)
        {
            this->_factories[it->first] = it->second;
        }
    }
    return *this;
}

ModuleFactoryPool& ModuleFactoryPool::load(const std::string& name, const std::string& path)
{
    this->_factories[name] = std::make_shared<ExternalFactoryAdapter>(path);
    return *this;
}
