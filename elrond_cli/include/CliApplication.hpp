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
                void onStateChanged(elrond::application::State) const override;

                void onInstanceCreating(const std::string& name, const std::string& factory) const override;
                void onInstanceCreated(elrond::interface::ModuleInstance& instance, elrond::interface::FactoryAdapter& factory) const override;
                void onInstanceSetup(elrond::interface::ModuleInstance& instance) const override;
                void onInstanceStart(elrond::interface::ModuleInstance& instance) const override;
                void onInstanceStop(elrond::interface::ModuleInstance& instance) const override;

            public:
                CliApplication( std::shared_ptr<ConfigInterface>& config,
                                CliConsoleAdapter& cliConsoleAdapter,
                                elrond::application::ModuleFactoryPool& factories);
        };
    }

#endif
