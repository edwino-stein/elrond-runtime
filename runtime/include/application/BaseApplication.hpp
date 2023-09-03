#ifndef ELROND_RUNTIME_APPLICATION_BASE_APPLICATION_HPP
    #define ELROND_RUNTIME_APPLICATION_BASE_APPLICATION_HPP

    #include "interface/Application.hpp"
    #include "application/ApplicationEvents.hpp"

    namespace elrond
    {
        namespace application
        {
            class BaseApplication : public elrond::interface::Application
            {
                protected:
                    std::map<std::string, elrond::application::ModuleInstanceP> _instances;
                    elrond::interface::ConsoleAdapter* _consoleAdapter;

                public:
                    ELROND_CLASS_SPECIAL_MEMBERS(BaseApplication, =delete, =delete, =delete, =delete, =delete)
                    BaseApplication(elrond::interface::ConsoleAdapter& consoleAdapter);

                    std::string name(elrond::interface::ModuleInstance& inst) const override;
                    elrond::pointer<elrond::interface::Console> console(elrond::interface::ModuleInstance& inst) const override;
                    elrond::pointer<elrond::interface::Arguments> arguments(elrond::interface::ModuleInstance& inst) const override;

                    elrond::interface::ModuleInstance& get(const std::string& name) const;
                    elrond::interface::ModuleInstance& insert(const std::string& name, elrond::interface::FactoryAdapter& factory);
                    bool exists(const std::string& name) const;
                    void each(elrond::procedure<elrond::interface::ModuleInstance&, const std::string&> handle) const;
            };
        }
    }

#endif
