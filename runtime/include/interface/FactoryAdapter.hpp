#ifndef ELROND_RUNTIME_INTERFACE_FACTORY_ADAPTER_HPP
    #define ELROND_RUNTIME_INTERFACE_FACTORY_ADAPTER_HPP

    #include "elrond_runtime_types.hpp"

    namespace elrond
    {
        namespace interface
        {
            class FactoryAdapter
            {
                public:
                    ELROND_DEFAULT_CLASS_SPECIAL_MEMBERS(FactoryAdapter)

                    virtual const elrond::platform::ModuleInfo& info() const =0;
                    virtual elrond::dWord apiVersion() const =0;

                    virtual elrond::application::InstanceHandleP make() const =0;
            };
        }
    }

#endif
