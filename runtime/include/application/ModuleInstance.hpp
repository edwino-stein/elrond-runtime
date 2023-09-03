#ifndef ELROND_RUNTIME_APPLICATION_MODULE_INSTANCE_HPP
    #define ELROND_RUNTIME_APPLICATION_MODULE_INSTANCE_HPP

    #include "interface/ModuleInstance.hpp"

    namespace elrond
    {
        namespace application
        {
            class ModuleInstance : public elrond::interface::ModuleInstance
            {
                protected:
                    elrond::interface::Application* _app;
                    elrond::application::InstanceHandleP _instance;
                    bool _enableLoop;
                    elrond::TimeSpan _loopTs;

                public:
                    ELROND_CLASS_SPECIAL_MEMBERS(ModuleInstance, =delete, =delete, =delete, =delete, =delete)
                    ModuleInstance(elrond::interface::Application& app, elrond::interface::FactoryAdapter& factory);

                    void setup() override;
                    void start() override;
                    void stop() override;

                    elrond::interface::Application& app() const override;

                    bool isEnabledLoop() const override;
                    void enableLoop(bool enable) override;
                    void loopTs(const elrond::TimeSpan& ts) override;

                    void loop() override;
                    elrond::TimeSpan timespan() const override;
            };
        }
    }
#endif
