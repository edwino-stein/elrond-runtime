#if !defined  _ELROND_RUNTIME_TYPES_HPP
    #define _ELROND_RUNTIME_TYPES_HPP

    #include "elrond.hpp"

    #ifndef GENERIC_STD_PLATFORM
        #error "The Elrond Test Library requires the GENERIC_STD_PLATFORM macro"
    #endif

    #ifndef ELROND_WITH_LAMBDA_TYPE
        #error "The Elrond Test Library requires the ELROND_WITH_LAMBDA_TYPE macro"
    #endif

    #ifndef ELROND_WITH_STR_TYPE
        #error "The Elrond Test Library requires the ELROND_WITH_STR_TYPE macro"
    #endif

    #ifndef ELROND_WITH_MODULES_INFO
        #error "The Elrond Test Library requires the ELROND_WITH_MODULES_INFO macro"
    #endif

    #include <vector>
    #include <memory>
    #include <algorithm>
    #include <thread>
    #include <mutex>
    #include <sstream>

    namespace elrond {
        namespace runtime {

            class RuntimeApp;
            class OStreamDebugOut;

            class ChannelManager;
            class RxChCollection;
            using ChannelManagerP = std::shared_ptr<ChannelManager>;
            using RxChCollectionP = std::unique_ptr<RxChCollection>;

            class CustomConfigMapAllocator;
            class DynamicConfigMemory;

            class ModuleHandle;
            class ModuleFactory;
            template<class T> class InternalModuleFactory;

            using ModuleFactoryP = std::shared_ptr<ModuleFactory>;
            using ModulesFactoriesV = std::vector<ModuleFactoryP>;
            using ModuleHandleP = std::shared_ptr<ModuleHandle>;

            class Exception;

            struct ModuleInfo {

                elrond::String mainClass;
                elrond::uInt32 apiVer;

                elrond::String prettyName;
                elrond::String authorName;
                elrond::String authorEmail;
                elrond::String version;

                elrond::String about() const;
                elrond::String name() const;

                unsigned int majorVer() const;
                unsigned int minorVer() const;
                unsigned int pathVer() const;
            };
        }
    }

#endif
