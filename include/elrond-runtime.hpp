#if !defined  _ELROND_RUNTIME_INCLUDE_HPP
    #define _ELROND_RUNTIME_INCLUDE_HPP

    #include "rtTypes.hpp"

    #include "application/RuntimeApp.hpp"

    #include "channel/ChannelManager.hpp"
    #include "channel/RxChCollection.hpp"

    #include "config/CustomConfigMapAllocator.hpp"
    #include "config/DynamicConfigMemory.hpp"

    #include "exceptions/Exception.hpp"

    #include "modules/ModuleHandle.hpp"
    #include "modules/ModuleFactory.hpp"
    #include "modules/DlModuleFactory.hpp"

    #include "modules/Serial.hpp"
    #include "modules/Udp.hpp"
    #include "modules/VirtualGpio.hpp"

    #include "util/Stacktrace.hpp"

#endif