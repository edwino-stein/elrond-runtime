#ifndef ELROND_RUNTIME_APPLICATION_APPLICATION_EVENTS_HPP
    #define ELROND_RUNTIME_APPLICATION_APPLICATION_EVENTS_HPP

    #include "elrond_runtime_types.hpp"

    namespace elrond
    {
        namespace application
        {
            class ApplicationEvents
            {
                private:
                    std::atomic<State> _state;

                protected:
                    void state(State state);

                    virtual void onStateChanged(State prev) const;
                    virtual void onInstanceCreating(const std::string& name, const std::string& factory) const;
                    virtual void onInstanceCreated(elrond::interface::ModuleInstance& instance, elrond::interface::FactoryAdapter& factory) const;
                    virtual void onInstanceSetup(elrond::interface::ModuleInstance& instance) const;
                    virtual void onInstanceStart(elrond::interface::ModuleInstance& instance) const;
                    virtual void onInstanceStop(elrond::interface::ModuleInstance& instance) const;

                public:
                    ELROND_CLASS_SPECIAL_MEMBERS_CONSTUCTORS(ApplicationEvents,, =delete, =delete, =delete, =delete)

                    State state() const;
            };
        }
    }

#endif
