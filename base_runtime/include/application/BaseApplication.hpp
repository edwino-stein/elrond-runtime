#ifndef ELROND_BASE_RUNTIME_APPLICATION_BASE_APPLICATION_HPP
    #define ELROND_BASE_RUNTIME_APPLICATION_BASE_APPLICATION_HPP

    #include "elrond_base_runtime_types.hpp"
    #include "interface/Application.hpp"

    namespace elrond
    {
        namespace application
        {
            class BaseApplication : public elrond::interface::Application
            {
                protected:

                    elrond::interface::ConsoleAdapter& _consoleAdapter;
                    const elrond::application::ModuleFactoryPool& _factories;
                    std::map<std::string, elrond::InstanceCtxP> _instances;

                    virtual void setup();
                    virtual void start();

                public:

                    BaseApplication(elrond::interface::ConsoleAdapter& consoleAdapter,
                                    const elrond::application::ModuleFactoryPool& factories);

                    elrond::pointer<elrond::interface::Console>
                    console(const std::string& name) const override;

                    elrond::pointer<elrond::interface::Arguments>
                    arguments(const std::string& name) const override;

                    elrond::platform::FactoryAdapterP
                    factory(const std::string& name) const override;

                    bool exists(const std::string& name) const;
                    elrond::InstanceCtxP get(const std::string& name) const;
                    elrond::InstanceCtxP add(const std::string& name, const std::string& factory);
                    void each(elrond::InstanceCtxH handle) const;

                    virtual void run();
            };
        }
    }

#endif
