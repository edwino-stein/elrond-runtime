#ifndef ELROND_RUNTIME_APPLICATION_INTERNAL_FACTORY_ADAPTER_HPP
    #define ELROND_RUNTIME_APPLICATION_INTERNAL_FACTORY_ADAPTER_HPP

    #include "application/FactoryAdapter.hpp"

    namespace elrond
    {
        namespace application
        {
            template <class M>
            class InternalFactoryAdapter : public elrond::application::FactoryAdapter
            {
                private:
                    elrond::platform::Factory<M> factoryInst;

                public:
                    ELROND_CLASS_SPECIAL_MEMBERS(InternalFactoryAdapter, =delete, =delete, =delete, =delete, =delete)
                    InternalFactoryAdapter(const elrond::platform::ModuleInfo& info);
            };
        }
    }

#endif
