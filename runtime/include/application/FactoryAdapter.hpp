#ifndef ELROND_RUNTIME_APPLICATION_FACTORY_ADAPTER_HPP
    #define ELROND_RUNTIME_APPLICATION_FACTORY_ADAPTER_HPP

    #include "interface/FactoryAdapter.hpp"

    namespace elrond
    {
        namespace application
        {
            class FactoryAdapter : public elrond::interface::FactoryAdapter
            {
                protected:
                    elrond::interface::Factory* _factory;

                public:
                    ELROND_CLASS_SPECIAL_MEMBERS(FactoryAdapter, =delete, =delete, =delete, =delete, =delete)
                    FactoryAdapter(elrond::interface::Factory* factory);

                    const elrond::platform::ModuleInfo& info() const override;
                    elrond::dWord apiVersion() const override;

                    elrond::application::InstanceHandleP make() const override;
            };
        }
    }

#endif
