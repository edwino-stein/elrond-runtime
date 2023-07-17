#include <elrond_mock.hpp>
#include <catch2.hpp>
#include "elrond_runtime.hpp"

using elrond::module::BaseGeneric;
using elrond::platform::ModuleInfo;
using elrond::application::FactoryAdapter;
using elrond::application::InternalFactoryAdapter;
using elrond::application::ExternalFactoryAdapter;
using elrond::platform::internalModuleFactories;
using elrond::module::HelloWorld;

const std::string TEST_BUILD_DIR = std::string(ELROND_BUILD_DIR) + "/runtime/test/";

SCENARIO("Test the module instance factory adapters", "[application][FactoryAdapter]")
{
    class TestModule : public BaseGeneric {};

    GIVEN("A factory adapter to ECL module")
    {
        FactoryAdapter adapter(internalModuleFactories()["elrond::HelloWorld"]);

        WHEN("Call the apiVersion getter method")
        {
            const auto apiVer = adapter.apiVersion();
            THEN("Should return the current elrond API version")
            {
                CHECK(apiVer == elrond::getApiVersion());
            }
        }

        WHEN("Call the info getter method")
        {
            auto& info = adapter.info();

            THEN("Should return the ModuleInfo values")
            {
                CHECK(info.name == "Hello World");
                CHECK(info.author == "Edwino Stein");
                CHECK(info.email == "edwino.stein@gmail.com");
                CHECK(info.version == "1.0.0");
            }
        }

        WHEN("Call the create method")
        {
            auto inst = adapter.make();

            THEN("Should return a Module instance")
            {
                CHECK(inst != nullptr);
                CHECK(isInstanceOf<BaseGeneric>(inst.get()));
                CHECK(isInstanceOf<HelloWorld>(inst.get()));
                CHECK(inst->moduleType() == elrond::ModuleType::GENERIC);
            }
        }
    }

    GIVEN("A internal factory adapter to generic module")
    {
        ModuleInfo info = {
            "Test Module",
            "Edwino Stein",
            "edwino.stein@gmail.com",
            "1.0"
        };

        InternalFactoryAdapter<TestModule> adapter(info);

        WHEN("Call the apiVersion getter method")
        {
            const auto apiVer = adapter.apiVersion();
            THEN("Should return the current elrond API version")
            {
                CHECK(apiVer == elrond::getApiVersion());
            }
        }

        WHEN("Call the info getter method")
        {
            auto& info = adapter.info();

            THEN("Should return the ModuleInfo values")
            {
                CHECK(info.name == "Test Module");
                CHECK(info.author == "Edwino Stein");
                CHECK(info.email == "edwino.stein@gmail.com");
                CHECK(info.version == "1.0");
            }
        }

        WHEN("Call the create method")
        {
            auto inst = adapter.make();

            THEN("Should return a Module instance")
            {
                CHECK(inst != nullptr);
                CHECK(isInstanceOf<BaseGeneric>(inst.get()));
                CHECK(isInstanceOf<TestModule>(inst.get()));
                CHECK(inst->moduleType() == elrond::ModuleType::GENERIC);
            }
        }
    }

    GIVEN("An external factory adapter")
    {
        ExternalFactoryAdapter adapter(TEST_BUILD_DIR + "misc/ExternalModule.so");

        WHEN("Call the apiVersion getter method")
        {
            const auto apiVer = adapter.apiVersion();
            THEN("Should return the current elrond API version")
            {
                CHECK(apiVer == elrond::getApiVersion());
            }
        }

        WHEN("Call the info getter method")
        {
            auto& info = adapter.info();

            THEN("Should return the ModuleInfo values")
            {
                CHECK(info.name == "External Test Module");
                CHECK(info.author == "Edwino Stein");
                CHECK(info.email == "edwino.stein@gmail.com");
                CHECK(info.version == "1.0.0");
            }
        }

        WHEN("Call the create method")
        {
            auto inst = adapter.make();

            THEN("Should return a Module instance")
            {
                CHECK(inst != nullptr);
                CHECK(isInstanceOf<BaseGeneric>(inst.get()));
                CHECK(inst->moduleType() == elrond::ModuleType::GENERIC);
            }
        }
    }
}
