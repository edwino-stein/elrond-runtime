#include <elrond_runtime.hpp>
#include <elrond_mock.hpp>
#include "catch2.hpp"

using elrond::application::ModuleFactoryPool;
using elrond::module::BaseGeneric;
using elrond::application::FactoriesM;
using elrond::platform::ModuleInfo;
using elrond::module::HelloWorld;

class TestModule : public BaseGeneric {};

SCENARIO("Test ModuleFactoryPool on normal usage", "[application][ModuleFactoryPool]")
{
    GIVEN("An empty ModuleFactoryPool instance")
    {
        ModuleFactoryPool factories;

        WHEN("Try to get factory 'test'")
        {
            THEN("Should throw an exception")
            {
                REQUIRE_THROWS_WITH(
                    factories.get("test"),
                    "Factory not found"
                );
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

                CHECK(fact->info().name == "NAME");
                CHECK(fact->info().author == "AUTHOR");
                CHECK(fact->info().email == "EMAIL");
                CHECK(fact->info().version == "VERSION");

                auto inst = fact->make();
                CHECK(isInstanceOf<TestModule>(inst.get()));
            }
        }

        WHEN("Load an external module factory called 'test'")
        {
            factories.load("test", "misc/ExternalModule");

            THEN("Try to get factory 'test'")
            {
                auto fact = factories.get("test");

                CHECK(fact->info().name == "External Test Module");
                CHECK(fact->info().author == "Edwino Stein");
                CHECK(fact->info().email == "edwino.stein@gmail.com");
                CHECK(fact->info().version == "1.0.0");

                auto inst = fact->make();
                CHECK(isInstanceOf<BaseGeneric>(inst.get()));
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
                CHECK(fact->info().name == "NAME");
                CHECK(fact->info().author == "AUTHOR");
                CHECK(fact->info().email == "EMAIL");
                CHECK(fact->info().version == "VERSION");

                auto inst = fact->make();
                CHECK(isInstanceOf<TestModule>(inst.get()));
            }
        }
    }

    GIVEN("The ModuleFactoryPool instance with internal common modules factories")
    {
        ModuleFactoryPool& factories = ModuleFactoryPool::internals();

        WHEN("Try to get factory 'elrond::HelloWorld'")
        {
            auto fact = factories.get("elrond::HelloWorld");
            THEN("Should return the factory object")
            {
                CHECK(fact->info().name == "Hello World");
                CHECK(fact->info().author == "Edwino Stein");
                CHECK(fact->info().email == "edwino.stein@gmail.com");
                CHECK(fact->info().version == "1.0.0");

                auto inst = fact->make();
                CHECK(isInstanceOf<HelloWorld>(inst.get()));
            }
        }
    }
}

SCENARIO("Test merging ModuleFactoryPool instances", "[application][ModuleFactoryPool]")
{
    GIVEN("A ModuleFactoryPool instance only passing the internal common modules factories")
    {
        ModuleFactoryPool factories(ModuleFactoryPool::internals());

        WHEN("Define a new module instance")
        {
            factories.define<TestModule> (
                "test",
                ModuleInfo {"NAME", "AUTHOR", "EMAIL", "VERSION"}
            );

            WHEN("Try to get factory 'test'")
            {
                auto fact = factories.get("test");
                
                THEN("Should return the factory object")
                {
                    CHECK(fact->info().name == "NAME");
                    CHECK(fact->info().author == "AUTHOR");
                    CHECK(fact->info().email == "EMAIL");
                    CHECK(fact->info().version == "VERSION");

                    auto inst = fact->make();
                    CHECK(isInstanceOf<TestModule>(inst.get()));
                }
            }

            AND_WHEN("Try to get factory 'elrond::HelloWorld'")
            {
                auto fact = factories.get("elrond::HelloWorld");
                THEN("Should return the factory object")
                {
                    CHECK(fact->info().name == "Hello World");
                    CHECK(fact->info().author == "Edwino Stein");
                    CHECK(fact->info().email == "edwino.stein@gmail.com");
                    CHECK(fact->info().version == "1.0.0");

                    auto inst = fact->make();
                    CHECK(isInstanceOf<HelloWorld>(inst.get()));
                }
            }

            AND_WHEN("Try to load a new module instance with existing name")
            {
                THEN("Should throw a exception")
                {
                    REQUIRE_THROWS_WITH (
                        factories.load("test", "misc/ExternalModule"),
                        "Module factory redefinition"
                    );
                }
            }

            WHEN("Try to define a new module instance with existing name of merged factory pool")
            {
                THEN("Should throw a exception")
                {
                    REQUIRE_THROWS_WITH (
                        factories.define<TestModule>("elrond::HelloWorld", ModuleInfo {"NAME", "AUTHOR", "EMAIL", "VERSION"}),
                        "Module factory redefinition"
                    );
                }
            }

            AND_WHEN("Try to load a new module instance with existing name of merged factory pool")
            {
                THEN("Should throw a exception")
                {
                    REQUIRE_THROWS_WITH (
                        factories.load("elrond::HelloWorld", "misc/ExternalModule"),
                        "Module factory redefinition"
                    );
                }
            }
        }
    }
}
