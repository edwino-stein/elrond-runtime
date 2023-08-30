#ifndef ELROND_RUNTIME_TYPES_HPP
    #define ELROND_RUNTIME_TYPES_HPP

    #include <elrond.hpp>
    #include <elrond_platform_runtime.hpp>
    #include <elrond_common_runtime.hpp>

    #include <future>
    #include <thread>
    #include <map>
    #include <queue>

    namespace elrond
    {
        namespace interface
        {
            class Application;
            class FactoryAdapter;
            class LoopTask;
            class ModuleInstance;
        }

        namespace application
        {
            enum class State
            {
                CREATED,
                INITIALIZING,
                INITIALIZED,
                STARTING,
                RUNNING,
                RUNNING_AND_STOPPING,
                STOPPING,
                STOPPING_FORCED,
                STOPPED
            };

            class FactoryAdapter;
            class ExternalFactoryAdapter;
            template <class M> class InternalFactoryAdapter;
            class ModuleFactoryPool;
            class ModuleInstance;
            class Context;

            class AsyncLoopTask;
            class ApplicationEvents;
            class BaseApplication;

            using FactoryP = std::shared_ptr<elrond::interface::FactoryAdapter>;
            using FactoriesM = std::map<std::string, FactoryP>;
            using InstanceHandleP = std::unique_ptr<elrond::interface::Module, std::function<void(elrond::interface::Module*)>>;
            using ModuleInstanceP = std::shared_ptr<elrond::application::ModuleInstance>;
            using AsyncLoopTaskP = std::shared_ptr<elrond::application::AsyncLoopTask>;
        }
    }

#endif
