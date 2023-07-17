#ifndef ELROND_RUNTIME_APPLICATION_EXTERNAL_FACTORY_ADAPTER_HPP
    #define ELROND_RUNTIME_APPLICATION_EXTERNAL_FACTORY_ADAPTER_HPP

    #include "application/FactoryAdapter.hpp"
    #include "platform/DlHandle.hpp"

    namespace elrond
    {
        namespace application
        {
            class ExternalFactoryAdapter : public elrond::application::FactoryAdapter
            {
                private:
                    elrond::platform::DlHandle _handle;

                public:
                    ELROND_CLASS_SPECIAL_MEMBERS(ExternalFactoryAdapter, =delete, =delete, =delete, =delete, =delete)
                    ExternalFactoryAdapter(const std::string& path);
            };
        }
    }

#endif
