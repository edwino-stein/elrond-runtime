#include "JsonConfig.hpp"
#include <fstream>

using elrond::JsonConfig;
using elrond::ConfigInterface;

JsonConfig::JsonConfig(nlohmann::json& root) : root(root) {}

void JsonConfig::eachFactory(ConfigInterface::EachHandle handle)
{ for (auto o : this->root["factories"].items()) handle(o.key(), o.value()); }

void JsonConfig::eachInstanceFactory(ConfigInterface::EachHandle handle)
{ for (auto o : this->root["instances"].items()) handle(o.key(), o.value()["factory"]); }

std::size_t JsonConfig::totalFactories() const
{ return this->root["factories"].size(); }

std::size_t JsonConfig::totalInstances() const
{ return this->root["instances"].size(); }

std::string JsonConfig::dumpConfig() const
{
    return "application/json\n" + this->root.dump(3);
}

void JsonConfig::validateFactories(nlohmann::json& root)
{
    if (!root.contains("factories"))
    {
        root["factories"] = nlohmann::json::object();
    }

    if (!root["factories"].is_object())
    {
        throw std::runtime_error("$.factories: Must be an object");
    }

    for (auto i : root["factories"].items())
    {
        if (!i.value().is_string())
        {
            throw std::runtime_error("$.factories."+ i.key() + ": Must be a stirng");
        }
    }
}

void JsonConfig::validateInstances(nlohmann::json& root)
{
    if (!root["instances"].is_object())
    {
        throw std::runtime_error("$.instances: Must be an object");
    }

    if (root["instances"].size() == 0)
    {
        throw std::runtime_error("$.instances: Must not be empty");
    }

    for (auto i : root["instances"].items())
    {
        if (i.value().is_string())
        {
            i.value() = nlohmann::json{{"factory", i.value()}};
        }

        if(i.value().is_object())
        {
            if (!i.value()["factory"].is_string())
            {
                throw std::runtime_error("$.instances."+ i.key() + ".factory: Must be a string");
            }

            if (!i.value().contains("arguments"))
            {
                i.value()["arguments"] = nlohmann::json::object();
            }

            JsonConfig::validateInstancesArgs(i.key(), i.value()["arguments"]);
            continue;
        }

        throw std::runtime_error("$.instances."+ i.key() + ": Must be an object or a string");
    }
}

void JsonConfig::validateInstancesArgs(const std::string& key, nlohmann::json& args)
{
    if (!args.is_object())
    {
        throw std::runtime_error("$.instances."+ key + ".arguments: Must be an object");
    }

    for (auto i : args.items())
    {
        switch (i.value().type())
        {
            case nlohmann::json::value_t::boolean:
            case nlohmann::json::value_t::string:
            case nlohmann::json::value_t::number_float:
            case nlohmann::json::value_t::number_integer:
            case nlohmann::json::value_t::number_unsigned:
                continue;
            break;

            default:
                throw std::runtime_error("$.instances."+ key + ".arguments." + i.key() + ": Must be an escalar type (bool, int, float or string)");
            break;
        }
    }
}

void JsonConfig::validate(nlohmann::json& root)
{
    JsonConfig::validateFactories(root);
    JsonConfig::validateInstances(root);
}

std::shared_ptr<elrond::ConfigInterface> JsonConfig::fromFile(const elrond::string& file)
{
    std::ifstream f(file);
    if (!f.is_open())
    {
        throw std::runtime_error(file + ": No such file or directory");
    }

    nlohmann::json root = nlohmann::json::parse(f);
    JsonConfig::validate(root);

    return std::make_shared<JsonConfig>(root);
}


