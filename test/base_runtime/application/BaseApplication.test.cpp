#include <elrond_runtime.hpp>
#include <elrond_mock.hpp>
#include <catch2.hpp>

using elrond::application::BaseApplication;
using elrond::application::ModuleFactoryPool;
using elrond::mock::ConsoleAdapter;
using elrond::module::BaseGeneric;
using elrond::platform::ModuleInfo;
using Catch::Matchers::Contains;

class TestModule : public BaseGeneric
{
    public:
        void setup(elrond::ContextP ctx)
        {
            ctx->console()->info("setup");
        }

        void start(elrond::ContextP ctx)
        {
            ctx->console()->info("start");
        }

        void loop(elrond::ContextP ctx)
        {
            ctx->console()->info("loop");
        }

        void stop(elrond::ContextP ctx)
        {
            ctx->console()->info("stop");
        }
};

class TestErrorModule : public TestModule
{
    public:
        void loop(elrond::ContextP ctx)
        {
            ctx->console()->error("This is an error");
        }
};

SCENARIO("Test the BaseApplication class just starting and stopping", "[application][BaseApplication]")
{
    std::ostringstream info;
    ConsoleAdapter adapter([&info](std::ostringstream& msg) { info << msg.str() << '\n'; });

    GIVEN("An instance of BaseApplication with an internal module factory")
    {
        ModuleFactoryPool factories;
        factories.define<TestModule>(
            "test",
            ModuleInfo {"NAME", "AUTHOR", "EMAIL", "VERSION"}
        );

        BaseApplication app(adapter, factories);

        WHEN("Try to create an instance of 'test'")
        {
            app.add("test", "test");

            THEN("The instance should be created")
            {
                REQUIRE(app.exists("test"));
                
                auto ctx = app.get("test");
                CHECK(ctx->name() == "test");
            }

            WHEN("Call run() method")
            {
                auto f = app.run();
                app.stop();
                f.get();

                THEN("The instance lifecycle methods shold be called properly")
                {
                    info.flush();
                    CHECK_THAT(info.str(), Contains("setup"));
                    CHECK_THAT(info.str(), Contains("start"));
                    CHECK_THAT(info.str(), !Contains("loop"));
                    CHECK_THAT(info.str(), Contains("stop"));
                }
            }
        }
    }

    GIVEN("An instance of BaseApplication with an externel module factory")
    {
        ModuleFactoryPool factories;
        factories.load("test", "ExternalModule");

        BaseApplication app(adapter, factories);

        WHEN("Try to create an instance of 'test'")
        {
            app.add("test", "test");

            THEN("The instance should be created")
            {
                REQUIRE(app.exists("test"));
                
                auto ctx = app.get("test");
                CHECK(ctx->name() == "test");
            }

            WHEN("Call run() method")
            {
                auto f = app.run();
                app.stop();
                f.get();
                THEN("The instance lifecycle methods shold be called properly")
                {
                    info.flush();
                    CHECK_THAT(info.str(), Contains("setup"));
                    CHECK_THAT(info.str(), Contains("start"));
                    CHECK_THAT(info.str(), !Contains("loop"));
                    CHECK_THAT(info.str(), Contains("stop"));
                }
            }
        }
    }
}

SCENARIO("Test the BaseApplication class just starting, runnig per 100ms and then stopping", "[application][BaseApplication]")
{
    std::ostringstream info;
    ConsoleAdapter adapter([&info](std::ostringstream& msg) { info << msg.str() << '\n'; });

    GIVEN("An instance of BaseApplication with an internal module factory")
    {
        ModuleFactoryPool factories;
        factories.define<TestModule>(
            "test",
            ModuleInfo {"NAME", "AUTHOR", "EMAIL", "VERSION"}
        );

        BaseApplication app(adapter, factories);

        WHEN("Add the module instance of 'test' with loop every 25ms")
        {
            app.add("test", "test");
            
            auto ctx = app.get("test");
            ctx->loopEnable(true);
            ctx->loopInterval(25);

            AND_WHEN("Run the application, wait 100ms and stop it")
            {
                auto f = app.run();
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                app.stop();
                f.get();

                THEN("The instance lifecycle methods shold be called properly")
                {
                    info.flush();
                    CHECK_THAT(info.str(), Contains("setup"));
                    CHECK_THAT(info.str(), Contains("start"));
                    CHECK_THAT(info.str(), Contains("loop"));
                    CHECK_THAT(info.str(), Contains("stop"));
                }
            }
        }
    }
}

SCENARIO("Test the BaseApplication class just starting, run and the module instance throws an error during loop", "[application][BaseApplication]")
{
    std::ostringstream info;
    ConsoleAdapter adapter([&info](std::ostringstream& msg) { info << msg.str() << '\n'; });

    GIVEN("An instance of BaseApplication with an internal module factory")
    {
        ModuleFactoryPool factories;
        factories.define<TestErrorModule>(
            "test",
            ModuleInfo {"NAME", "AUTHOR", "EMAIL", "VERSION"}
        );

        BaseApplication app(adapter, factories);

        WHEN("Add the module instance of 'test' with loop every 25ms")
        {
            app.add("test", "test");

            auto ctx = app.get("test");
            ctx->loopEnable(true);
            ctx->loopInterval(25);

            AND_WHEN("Run the application, wait 100ms and stop it")
            {
                auto f = app.run();

                THEN("Shold throw an error")
                {
                    try {
                        f.get();
                        FAIL();
                    }
                    catch (const std::exception& e)
                    {
                        CHECK(std::string(e.what()) == "This is an error");
                        SUCCEED();
                    }
                    app.stop();

                    THEN("The instance lifecycle methods shold be called properly")
                    {
                        info.flush();
                        CHECK_THAT(info.str(), Contains("setup"));
                        CHECK_THAT(info.str(), Contains("start"));
                        CHECK_THAT(info.str(), !Contains("loop"));
                        CHECK_THAT(info.str(), Contains("stop"));
                    }
                }
            }
        }
    }
}
