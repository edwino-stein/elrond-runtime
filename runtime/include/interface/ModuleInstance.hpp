#ifndef ELROND_RUNTIME_INTERFACE_MODULE_INSTANCE_HPP
    #define ELROND_RUNTIME_INTERFACE_MODULE_INSTANCE_HPP

    #include "elrond_runtime_types.hpp"
    #include "interface/LoopTask.hpp"

    namespace elrond
    {
        namespace interface
        {
            class ModuleInstance : public elrond::interface::LoopTask
            {
                public:
                    ELROND_DEFAULT_CLASS_SPECIAL_MEMBERS(ModuleInstance)

                    virtual void setup() =0;
                    virtual void start() =0;
                    virtual void stop() =0;

                    virtual elrond::interface::Application& app() const =0;

                    virtual bool isEnabledLoop() const =0;
                    virtual void enableLoop(bool enable) =0;
                    virtual void loopTs(const elrond::TimeSpan& ts) =0;
            };
        }
    }
#endif
