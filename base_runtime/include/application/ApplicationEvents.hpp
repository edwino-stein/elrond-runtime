#ifndef ELROND_BASE_RUNTIME_APPLICATION_APPLICATION_EVENTS_HPP
    #define ELROND_BASE_RUNTIME_APPLICATION_APPLICATION_EVENTS_HPP

    #include "elrond_base_runtime_types.hpp"

    namespace elrond
    {
        namespace application
        {
            class ApplicationEvents
            {
                private:

                    std::atomic<State> _state;

                protected:

                    ApplicationEvents();

                    void state(State state);

                    virtual void onStateChange(State next) const;
                    virtual void onInstanceCreating(const std::string& name, const std::string& factory) const;
                    virtual void onInstanceCreated(const elrond::application::InstanceCtx& ctx) const;
                    virtual void onInstanceSetup(const elrond::application::InstanceCtx& ctx) const;
                    virtual void onInstanceStart(const elrond::application::InstanceCtx& ctx) const;
                    virtual void onInstanceStop(const elrond::application::InstanceCtx& ctx) const;

                public:

                    virtual ~ApplicationEvents() = default;

                    State state() const;
            };
        }
    }

#endif
