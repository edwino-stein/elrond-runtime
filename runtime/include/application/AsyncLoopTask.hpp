#ifndef ELROND_RUNTIME_APPLICATION_ASYNC_LOOP_TASK_HPP
    #define ELROND_RUNTIME_APPLICATION_ASYNC_LOOP_TASK_HPP

    #include "elrond_runtime_types.hpp"

    namespace elrond
    {
        namespace application
        {
                class AsyncLoopTask
                {
                    public:

                        enum class STATUS
                        {
                            NOT_STARTED,
                            STARING,
                            RUNNING,
                            SLEEPING,
                            STOPPING,
                            STOPPED,
                            FINISHED
                        };

                    protected:

                        elrond::interface::LoopTask* _task;
                        std::atomic<STATUS> _status;
                        std::thread _thread;
                        std::unique_ptr<std::future<void>> _future;
                        bool _detached;

                    public:
                        ELROND_CLASS_SPECIAL_MEMBERS_CONSTUCTORS(AsyncLoopTask, =delete, =delete, =delete, =delete, =delete)
                        AsyncLoopTask(elrond::interface::LoopTask& task);
                        ~AsyncLoopTask();

                        void run();
                        void stop();

                        bool isRunning() const;
                        bool isStopping() const;
                        bool wasDetached() const;

                        void finish(bool force);

                    protected:
                        static void loop(AsyncLoopTask* self, std::promise<void> promise);
                };
        }
    }

#endif
