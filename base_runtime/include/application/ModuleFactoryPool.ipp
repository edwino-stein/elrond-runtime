#ifndef ELROND_BASE_RUNTIME_APPLICATION_MODULE_FACTORY_POOL_IPP
    #define ELROND_BASE_RUNTIME_APPLICATION_MODULE_FACTORY_POOL_IPP

    #include "application/ModuleFactoryPool.hpp"

    namespace elrond
    {
        namespace application
        {
            template <class T>
            ModuleFactoryPool& ModuleFactoryPool::define(
                const std::string& name,
                const elrond::platform::ModuleInfo& info
            ){
                auto f = std::make_shared<elrond::platform::InternalFactoryAdapter<T>>(info);
                this->_factories[name] = f;
                return *this;
            }
        }
    }

#endif
