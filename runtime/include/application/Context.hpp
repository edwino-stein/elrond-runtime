#ifndef ELROND_RUNTIME_APPLICATION_CONTEXT_HPP
    #define ELROND_RUNTIME_APPLICATION_CONTEXT_HPP

    #include "elrond_runtime_types.hpp"

    namespace elrond
    {
        namespace application
        {
            class Context : public elrond::interface::Context
            {
                protected:
                    elrond::interface::ModuleInstance& _instance;

                public:
                    ELROND_CLASS_SPECIAL_MEMBERS(Context, =delete, =delete, =delete, =delete, =delete)
                    Context(elrond::interface::ModuleInstance& instance);

                    elrond::pointer<elrond::interface::Console> console() const override;
                    elrond::pointer<elrond::interface::Arguments> arguments() const override;
                    elrond::string name() const override;

                    void setLoopEvery(const elrond::TimeSpan& ts) override;
                    void unsetLoop() override;
            };
        }
    }

#endif
