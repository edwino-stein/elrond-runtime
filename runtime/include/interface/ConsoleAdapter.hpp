#ifndef ELROND_RUNTIME_INTERFACE_CONSOLE_ADAPTER_HPP
    #define ELROND_RUNTIME_INTERFACE_CONSOLE_ADAPTER_HPP

     #include "elrond_runtime_types.hpp"

    namespace elrond
    {
        namespace interface
        {
            class ConsoleAdapter
            {
                public:
                    ELROND_DEFAULT_CLASS_SPECIAL_MEMBERS(ConsoleAdapter)
                    virtual elrond::pointer<ConsoleStreamAdapter> makeConsoleStreamAdapter(const std::string& tag)=0;
            };
        }
    }

#endif
