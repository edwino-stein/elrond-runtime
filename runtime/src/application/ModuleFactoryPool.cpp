#include "application/ModuleFactoryPool.hpp"
#include "application/ExternalFactoryAdapter.hpp"
#include "application/InternalFactoryAdapter.ipp"

using elrond::application::ModuleFactoryPool;
using elrond::application::FactoriesM;
using elrond::application::FactoryP;
using elrond::application::FactoryAdapter;
using elrond::application::ExternalFactoryAdapter;
using elrond::platform::internalModuleFactories;

ModuleFactoryPool& ModuleFactoryPool::internals()
{
    static ModuleFactoryPool factories;

    if (factories.size() == 0)
    {
        auto& internals = internalModuleFactories();
        for (auto it = internals.cbegin(); it != internals.cend(); it++)
        {
            auto f = std::make_shared<FactoryAdapter>(it->second);
            factories.add(it->first, f);
        }
    }

    return factories;
}

ModuleFactoryPool::ModuleFactoryPool()
: _factories(), _next(nullptr) {}

ModuleFactoryPool::ModuleFactoryPool(ModuleFactoryPool& next)
: _factories(), _next(&next) {}

ModuleFactoryPool::ModuleFactoryPool(const FactoriesM& factories)
: _factories(factories), _next(nullptr) {}

ModuleFactoryPool::ModuleFactoryPool(const FactoriesM& factories, ModuleFactoryPool& next)
: _factories(factories), _next(&next) {}

void ModuleFactoryPool::add(const std::string& name, elrond::application::FactoryP factory)
{
    if (this->exists(name))
    {
        throw std::runtime_error("Module factory redefinition");
    }

    this->_factories[name] = factory;
}

FactoryP ModuleFactoryPool::get(const std::string& name) const
{
    try
    {
        return this->_factories.at(name);
    }
    catch(const std::out_of_range&)
    {
        if (this->_next == nullptr)
        {
            throw std::runtime_error("Factory not found");
        }
    }

    return this->_next->get(name);
}

bool ModuleFactoryPool::exists(const std::string& name) const
{
    try
    {
        this->get(name);
        return true;
    }
    catch(const std::exception& e)
    {
        return false;
    }
}

elrond::sizeT ModuleFactoryPool::size() const
{
    return this->_factories.size();
}

FactoryP ModuleFactoryPool::load(const std::string& name, const std::string& path)
{
    auto f = std::make_shared<ExternalFactoryAdapter>(path);
    this->add(name, f);
    return f;
}
