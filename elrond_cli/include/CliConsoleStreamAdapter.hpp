#ifndef ELROND_CLI_CONSOLE_STREAM_ADAPTER_HPP
    #define ELROND_CLI_CONSOLE_STREAM_ADAPTER_HPP

    #include <elrond_runtime.hpp>

    namespace elrond
    {
        class CliConsoleAdapter;
        class CliConsoleStreamAdapter : public elrond::interface::ConsoleStreamAdapter
        {
            protected:
                CliConsoleAdapter* _adapter;
                std::string _tag;
                std::unique_ptr<elrond::runtime::OStream> _stream;

            public:
                ELROND_DEFAULT_CLASS_SPECIAL_MEMBERS(CliConsoleStreamAdapter)
                CliConsoleStreamAdapter(CliConsoleAdapter& adapter, const std::string& tag);

                void flush(elrond::interface::Stream& stream, elrond::SEVERITY severity) override;
                elrond::pointer<elrond::interface::Stream> makeStream() const override;
        };
    }

#endif
