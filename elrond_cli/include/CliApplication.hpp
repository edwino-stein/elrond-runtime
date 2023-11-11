#ifndef ELROND_CLI_APPLICATION_HPP
    #define ELROND_CLI_APPLICATION_HPP

    #include <elrond_runtime.hpp>

    namespace elrond
    {
        class CliConsoleAdapter;
        class ConfigInterface;

        class CliApplication : public elrond::application::RuntimeApplication
        {
            protected:
                std::shared_ptr<ConfigInterface> config;
                CliConsoleAdapter& cliConsoleAdapter;

                elrond::pointer<elrond::interface::Console> console() const;

            public:
                CliApplication( std::shared_ptr<ConfigInterface>& config,
                                CliConsoleAdapter& cliConsoleAdapter,
                                elrond::application::ModuleFactoryPool& factories);
        };
    }

#endif
