#ifndef ELROND_BASE_RUNTIME_INTERFACE_APPLICATION_HPP
    #define ELROND_BASE_RUNTIME_INTERFACE_APPLICATION_HPP

    #include "elrond_base_runtime_types.hpp"

    namespace elrond
    {
        namespace interface
        {
            class Application
            {
                public:
                    virtual ~Application() = default;

                    virtual elrond::pointer<elrond::interface::Console>
                    console(const std::string& name) const =0;

                    virtual elrond::pointer<elrond::interface::Arguments>
                    arguments(const std::string& name) const =0;

                    virtual elrond::platform::FactoryAdapterP
                    factory(const std::string& name) const =0;
            };
        }
    }

#endif
