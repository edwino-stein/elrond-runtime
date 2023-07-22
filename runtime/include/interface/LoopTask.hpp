#ifndef ELROND_RUNTIME_INTERFACE_LOOP_TASK_HPP
    #define ELROND_RUNTIME_INTERFACE_LOOP_TASK_HPP

    #include "elrond_runtime_types.hpp"

    namespace elrond
    {
        namespace interface
        {
            class LoopTask
            {
                public:
                    ELROND_DEFAULT_CLASS_SPECIAL_MEMBERS(LoopTask)

                    virtual void loop() =0;
                    virtual elrond::TimeSpan timespan() const =0;
            };
        }
    }

#endif
