#include <elrond_runtime.hpp>
#include <elrond_mock.hpp>
#include "catch2.hpp"

using elrond::application::ModuleFactoryPool;
using elrond::module::BaseGeneric;
using elrond::platform::ModuleInfo;
using elrond::platform::FactoriesM;

class TestModule : public BaseGeneric {};

SCENARIO("Test ModuleFactoryPool on normal usage", "[application][ModuleFactoryPool]")
{
    GIVEN("An empty ModuleFactoryPool instance")
    {
        ModuleFactoryPool factories;

        WHEN("Try to get factory 'test'")
        {
            try
            {
                factories.get("test");
                THEN("Should throw an exception")
                {
                    FAIL();
                }
            }
            catch(const std::exception& e)
            {
                THEN("Should throw an exception")
                {
                    REQUIRE(std::string(e.what()) == "Factory not found");
                }
            }
        }

        WHEN("Define a module factory called 'test'")
        {
            factories.define<TestModule>(
                "test",
                ModuleInfo {"NAME", "AUTHOR", "EMAIL", "VERSION"}
            );

            THEN("Try to get factory 'test'")
            {
                auto fact = factories.get("test");

                CHECK(fact->infoName() == "NAME");
                CHECK(fact->infoAuthor() == "AUTHOR");
                CHECK(fact->infoEmail() == "EMAIL");
                CHECK(fact->infoVersion() == "VERSION");

                auto inst = fact->create("inst");
                CHECK(isInstanceOf<TestModule>(inst->instance()));
            }
        }
    }

    GIVEN("A ModuleFactoryPool instance created using factories map")
    {
        ModuleFactoryPool factories (
            FactoriesM {
                {
                    ELROND_DEF_INTERNAL_MOD_FACTORY(
                        "test",
                        TestModule,
                        "NAME",
                        "AUTHOR",
                        "EMAIL",
                        "VERSION"
                    )
                }
            }
        );

        WHEN("Try to get factory 'test'")
        {
            auto fact = factories.get("test");
            THEN("Should return the factory object")
            {
                CHECK(fact->infoName() == "NAME");
                CHECK(fact->infoAuthor() == "AUTHOR");
                CHECK(fact->infoEmail() == "EMAIL");
                CHECK(fact->infoVersion() == "VERSION");

                auto inst = fact->create("inst");
                CHECK(isInstanceOf<TestModule>(inst->instance()));
            }
        }
    }

    GIVEN("A ModuleFactoryPool instance with internal common modules factories")
    {
        ModuleFactoryPool factories = ModuleFactoryPool::createWithCommonInternals();

        WHEN("Try to get factory 'elrond::HelloWorld'")
        {
            auto fact = factories.get("elrond::HelloWorld");
            THEN("Should return the factory object")
            {
                CHECK(fact->infoName() == "Hello World");
                CHECK(fact->infoAuthor() == "Edwino Stein");
                CHECK(fact->infoEmail() == "edwino.stein@gmail.com");
                CHECK(fact->infoVersion() == "1.0.0");

                auto inst = fact->create("inst");
                CHECK(isInstanceOf<elrond::module::HelloWorld>(inst->instance()));
            }
        }
    }
}

SCENARIO("Test merging ModuleFactoryPool instances", "[application][ModuleFactoryPool]")
{
    GIVEN("An empty ModuleFactoryPool instance")
    {
        ModuleFactoryPool factories;

        WHEN("Merge the common internal module factories")
        {
            factories.copyFrom(ModuleFactoryPool::createWithCommonInternals());

            WHEN("Try to get factory 'elrond::HelloWorld'")
            {
                auto fact = factories.get("elrond::HelloWorld");
                THEN("Should return the factory object")
                {
                    CHECK(fact->infoName() == "Hello World");
                    CHECK(fact->infoAuthor() == "Edwino Stein");
                    CHECK(fact->infoEmail() == "edwino.stein@gmail.com");
                    CHECK(fact->infoVersion() == "1.0.0");

                    auto inst = fact->create("inst");
                    CHECK(isInstanceOf<elrond::module::HelloWorld>(inst->instance()));
                }
            }
        }

        WHEN("Merge map of factories")
        {
            factories.copyFrom(
                FactoriesM {
                    {
                        ELROND_DEF_INTERNAL_MOD_FACTORY(
                            "test",
                            TestModule,
                            "NAME",
                            "AUTHOR",
                            "EMAIL",
                            "VERSION"
                        )
                    }
                }
            );

            WHEN("Try to get factory 'test'")
            {
                auto fact = factories.get("test");
                THEN("Should return the factory object")
                {
                    CHECK(fact->infoName() == "NAME");
                    CHECK(fact->infoAuthor() == "AUTHOR");
                    CHECK(fact->infoEmail() == "EMAIL");
                    CHECK(fact->infoVersion() == "VERSION");

                    auto inst = fact->create("inst");
                    CHECK(isInstanceOf<TestModule>(inst->instance()));
                }
            }
        }
    }

    GIVEN("An ModuleFactoryPool instance with common module factories")
    {
        ModuleFactoryPool factories = ModuleFactoryPool::createWithCommonInternals();

        WHEN("Merge from map with same name, but not allow to override")
        {
            factories.copyFrom(
                FactoriesM {
                    {
                        ELROND_DEF_INTERNAL_MOD_FACTORY(
                            "elrond::HelloWorld",
                            TestModule,
                            "NAME",
                            "AUTHOR",
                            "EMAIL",
                            "VERSION"
                        )
                    }
                },
                false
            );

            WHEN("Try to get factory 'elrond::HelloWorld'")
            {
                auto fact = factories.get("elrond::HelloWorld");
                THEN("Should return the factory object of internal module")
                {
                    CHECK(fact->infoName() == "Hello World");
                    CHECK(fact->infoAuthor() == "Edwino Stein");
                    CHECK(fact->infoEmail() == "edwino.stein@gmail.com");
                    CHECK(fact->infoVersion() == "1.0.0");

                    auto inst = fact->create("inst");
                    CHECK(isInstanceOf<elrond::module::HelloWorld>(inst->instance()));
                }
            }
        }

        WHEN("Merge from map with same name, but allow to override")
        {
            factories.copyFrom(
                FactoriesM {
                    {
                        ELROND_DEF_INTERNAL_MOD_FACTORY(
                            "elrond::HelloWorld",
                            TestModule,
                            "NAME",
                            "AUTHOR",
                            "EMAIL",
                            "VERSION"
                        )
                    }
                },
                true
            );

            WHEN("Try to get factory 'elrond::HelloWorld'")
            {
                auto fact = factories.get("elrond::HelloWorld");
                THEN("Should return the overrided factory object")
                {
                    CHECK(fact->infoName() == "NAME");
                    CHECK(fact->infoAuthor() == "AUTHOR");
                    CHECK(fact->infoEmail() == "EMAIL");
                    CHECK(fact->infoVersion() == "VERSION");

                    auto inst = fact->create("inst");
                    CHECK(isInstanceOf<TestModule>(inst->instance()));
                }
            }
        }
    }
}

SCENARIO("Test load external module factory using ModuleFactoryPool", "[application][ModuleFactoryPool]")
{
    GIVEN("An empty ModuleFactoryPool instance")
    {
        ModuleFactoryPool factories;

        WHEN("Try to load the 'ExternalModule' example external module")
        {
            factories.load("external", "ExternalModule");
            THEN("Try to get external module factory")
            {
                auto fact = factories.get("external");

                CHECK(fact->infoName() == "External Test Module");
                CHECK(fact->infoAuthor() == "Edwino Stein");
                CHECK(fact->infoEmail() == "edwino.stein@gmail.com");
                CHECK(fact->infoVersion() == "1.0.0");

                auto inst = fact->create("inst");
                CHECK(isInstanceOf<BaseGeneric>(inst->instance()));
            }
        }
    }
}
