#ifndef ELROND_RUNTIME_APPLICATION_MODULE_FACTORY_POOL_IPP
    #define ELROND_RUNTIME_APPLICATION_MODULE_FACTORY_POOL_IPP

    #include "application/ModuleFactoryPool.hpp"
    #include "application/InternalFactoryAdapter.ipp"

    namespace elrond
    {
        namespace application
        {
            template <class T>
            elrond::application::FactoryP ModuleFactoryPool::define (
                const std::string& name,
                const elrond::platform::ModuleInfo& info
            ){
                auto f = std::make_shared<elrond::application::InternalFactoryAdapter<T>>(info);
                this->add(name, f);
                return f;
            }
        }
    }

#endif
