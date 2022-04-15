#ifndef ELROND_BASE_RUNTIME_APPLICATION_INSTANCE_CONTEXT_HPP
    #define ELROND_BASE_RUNTIME_APPLICATION_INSTANCE_CONTEXT_HPP

    #include "elrond_base_runtime_types.hpp"

    namespace elrond
    {
        namespace application
        {
            class InstanceCtx : public elrond::interface::Context,
                                public std::enable_shared_from_this<InstanceCtx>
            {
                protected:

                    elrond::platform::ModuleInstanceP _instance;
                    const elrond::interface::Application& _app;

                    bool _loopEnabled;
                    bool _loopAsync;
                    elrond::timeT _loopInterval;

                    InstanceCtx(elrond::platform::ModuleInstanceP inst,
                                const elrond::interface::Application& app);
                public:

                    elrond::pointer<elrond::interface::Console> console() const override;
                    elrond::pointer<elrond::interface::Arguments> arguments() const override;
                    elrond::string name() const override;

                    void loopEnable(bool enable) override;
                    void loopInterval(elrond::timeT interval) override;
                    void loopAsync(bool enable) override;

                    bool loopEnable() const;
                    elrond::timeT loopInterval() const;
                    bool loopAsync() const;

                    void setup();
                    void start();
                    void loop();
                    void stop();

                    static elrond::InstanceCtxP make(const std::string& name,
                                                     const std::string& factory,
                                                     const elrond::interface::Application& app);

            };
        }
    }

#endif
