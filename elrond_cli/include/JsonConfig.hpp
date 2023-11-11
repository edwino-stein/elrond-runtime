#ifndef ELROND_JSON_CONFIG_HPP
    #define ELROND_JSON_CONFIG_HPP

    #include "ConfigInterface.hpp"
    #include <nlohmann/json.hpp>

    namespace elrond
    {
        class JsonConfig : public elrond::ConfigInterface
        {
            protected:
                nlohmann::json root;

                static void validateFactories(nlohmann::json& root);
                static void validateInstances(nlohmann::json& root);
                static void validateInstancesArgs(const std::string& key, nlohmann::json& args);
                static void validate(nlohmann::json& root);

            public:
                ELROND_CLASS_SPECIAL_MEMBERS(JsonConfig, =delete, =delete, =delete, =delete, =delete)
                JsonConfig(nlohmann::json& root);

                void eachFactory(EachHandle handle) override;
                void eachInstanceFactory(EachHandle handle) override;

                std::size_t totalFactories() const override;
                std::size_t totalInstances() const override;
                std::string dumpConfig() const override;

                static std::shared_ptr<elrond::ConfigInterface> fromFile(const elrond::string& file);
        };
    }

#endif
