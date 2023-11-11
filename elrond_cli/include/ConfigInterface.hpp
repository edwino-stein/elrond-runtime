#ifndef ELROND_CONFIG_INTERFACE_HPP
    #define ELROND_CONFIG_INTERFACE_HPP

    #include <elrond_runtime.hpp>

    namespace elrond
    {
        class ConfigInterface
        {
            public:
                using EachHandle = std::function<void(const std::string&, const std::string&)>;

                ELROND_DEFAULT_CLASS_SPECIAL_MEMBERS(ConfigInterface)

                virtual void eachFactory(EachHandle handle)=0;
                virtual void eachInstanceFactory(EachHandle handle)=0;

                virtual std::size_t totalFactories() const =0;
                virtual std::size_t totalInstances() const =0;
                virtual std::string dumpConfig() const =0;
        };
    }

#endif
