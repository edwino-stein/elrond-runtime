#ifndef ELROND_RUNTIME_INTERFACE_APPLICATION_HPP
    #define ELROND_RUNTIME_INTERFACE_APPLICATION_HPP

    #include "elrond_runtime_types.hpp"

    namespace elrond
    {
        namespace interface
        {
            class Application
            {
                public:
                    ELROND_DEFAULT_CLASS_SPECIAL_MEMBERS(Application)

                    virtual std::string name(elrond::interface::ModuleInstance& inst) const =0;
                    virtual elrond::pointer<elrond::interface::Console> console(elrond::interface::ModuleInstance& inst) const =0;
                    virtual elrond::pointer<elrond::interface::Arguments> arguments(elrond::interface::ModuleInstance& inst) const =0;
            };
        }
    }

#endif
