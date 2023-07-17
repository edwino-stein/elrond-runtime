#ifndef ELROND_RUNTIME_APPLICATION_INTERNAL_FACTORY_ADAPTER_IPP
    #define ELROND_RUNTIME_APPLICATION_INTERNAL_FACTORY_ADAPTER_IPP

    #include "application/InternalFactoryAdapter.hpp"
    
    namespace elrond
    {
        namespace application
        {
            template <class M>
            InternalFactoryAdapter<M>::InternalFactoryAdapter(const elrond::platform::ModuleInfo& info)
            : elrond::application::FactoryAdapter(&factoryInst), factoryInst(info) {}
        }
    }

#endif
