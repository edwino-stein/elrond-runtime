#ifndef ELROND_CLI_CONSOLE_ADAPTER_HPP
    #define ELROND_CLI_CONSOLE_ADAPTER_HPP

    #include <elrond_runtime.hpp>

    namespace elrond
    {
        class CliConsoleAdapter : public elrond::interface::ConsoleAdapter
        {
            protected:
                std::mutex coutMtx;
                std::vector<std::ostream*> outputStreams;
                std::size_t tagWidth;

            public:
                ELROND_CLASS_SPECIAL_MEMBERS(CliConsoleAdapter,,=delete, =delete, =delete, =delete)
                void flush(const std::string& tag, elrond::SEVERITY severity, const std::string& msg);
                elrond::pointer<elrond::interface::Console> makeConsole(const std::string& tag);
                elrond::pointer<elrond::interface::ConsoleStreamAdapter> makeConsoleStreamAdapter(const std::string& tag) override;
        };
    }

#endif
