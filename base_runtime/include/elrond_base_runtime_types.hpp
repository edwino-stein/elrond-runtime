#ifndef ELROND_BASE_RUNTIME_TYPES_HPP
    #define ELROND_BASE_RUNTIME_TYPES_HPP

    #include <elrond.hpp>
    #include <elrond_platform_runtime.hpp>
    #include <elrond_common_runtime.hpp>

    #include <map>

    namespace elrond
    {
        namespace interface
        {
            class Application;
        }
    
        namespace platform
        {
            using FactoriesM = std::map<std::string, elrond::platform::FactoryAdapterP>;
        }

        namespace application
        {
            class ModuleFactoryPool;
            class InstanceCtx;
            class BaseApplication;
        }

        namespace runtime
        {
            class NullArguments;
        }

        using InstanceCtxP = std::shared_ptr<application::InstanceCtx>;
        using InstanceCtxH = std::function<void(InstanceCtxP)>;
    }

#endif
