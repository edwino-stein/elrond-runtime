#if !defined  _ELROND_RUNTIME_APP_HPP
    #define _ELROND_RUNTIME_APP_HPP

    #include "rtTypes.hpp"

    namespace elrond {
        namespace runtime {

            class RuntimeApp : public elrond::interface::Runtime {

                protected:

                    elrond::interface::DebugOut &_dout;

                    std::vector<elrond::runtime::ModuleHandleP> instances;
                    std::vector<elrond::runtime::ChannelManagerP> chmgrs;

                    ModuleHandleP findModule(elrond::String name) const;
                    void startModules();
                    void stopModules();

                    static elrond::runtime::ModuleFactoryP findFactory(
                        elrond::String& name,
                        elrond::runtime::ModulesFactoriesV& factories,
                        elrond::interface::Runtime* app
                    );

                public:

                    RuntimeApp(elrond::interface::DebugOut& dout);
                    virtual ~RuntimeApp();

                    ModuleInfo const& defineModule(
                        elrond::String name,
                        elrond::String type,
                        elrond::runtime::ModulesFactoriesV& factories
                    );

                    void initModule(
                        elrond::String name,
                        elrond::interface::ConfigMap& cm
                    ) const;

                    elrond::runtime::ChannelManagerP defineChannelManager(
                        elrond::String transport,
                        const elrond::sizeT tx,
                        const elrond::sizeT rx,
                        const elrond::sizeT fps
                    );

                    void start();
                    void loop(ELROND_LAMBDA_FUNC(bool, void) continueHandle);
                    void stop(const bool force = false);

                    elrond::module::BaseGpioModule& getGpioService() const override;
                    elrond::module::BaseInputDriverModule& getInputService(const elrond::sizeT id = 0) const override;
                    elrond::channel::BaseChannelManager& getChannelManager(const elrond::sizeT id = 0) const override;
                    const elrond::interface::DebugOut& dout() const override;

                    void onError(const char* error) override;
                    void onError(elrond::String error) override;

                    static elrond::runtime::ModulesFactoriesV newModulesFactories();
            };
        }
    }

#endif
