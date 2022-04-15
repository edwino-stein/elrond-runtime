#include <elrond_runtime.hpp>
#include <elrond_mock.hpp>
#include "catch2.hpp"

using elrond::application::InstanceCtx;
using elrond::interface::Application;
using elrond::module::BaseGeneric;
using elrond::application::ModuleFactoryPool;
using elrond::platform::ModuleInfo;
using elrond::runtime::Console;
using elrond::mock::ConsoleAdapter;
using elrond::runtime::NullArguments;

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

struct TestApp : public Application
{
    ConsoleAdapter adapter;
    ModuleFactoryPool factories;

    TestApp(ConsoleAdapter::CallbackStream handle)
    : adapter(handle), factories() {}

    elrond::pointer<elrond::interface::Console>
    console(const std::string& name) const
    {
        return std::make_shared<Console>(
            name,
            const_cast<TestApp*>(this)->adapter
        );
    }

    elrond::pointer<elrond::interface::Arguments>
    arguments(const std::string&) const
    {
        return std::make_shared<NullArguments>();
    }

    elrond::platform::FactoryAdapterP
    factory(const std::string& name) const
    {
        return this->factories.get(name);
    }
};

SCENARIO("Test InstanceCtx of internal module on normal usage", "[application][InstanceCtx]")
{
    std::ostringstream info;
    TestApp app([&info](std::ostringstream& msg) {info << msg.str(); });

    GIVEN("An TestModule instance context")
    {
        app.factories.define<TestModule>(
            "test",
            ModuleInfo {"NAME", "AUTHOR", "EMAIL", "VERSION"}
        );

        auto ctx = InstanceCtx::make("test", "test", app);

        WHEN("Call name() method")
        {
            auto name = ctx->name();
            THEN("Should return the instance name")
            {
                CHECK(name == "test");
            }
        }

        WHEN("Call console() method")
        {
            auto console = ctx->console();
            THEN("Should return the console instance")
            {
                CHECK(dynamic_cast<Console*>(console.get()) != nullptr);
            }
        }

        WHEN("Call arguments() method")
        {
            auto args = ctx->arguments();
            THEN("Should return the arguments instance")
            {
                CHECK(dynamic_cast<NullArguments*>(args.get()) != nullptr);
            }
        }

        WHEN("Set loop parameters")
        {
            REQUIRE_FALSE(ctx->loopEnable());
            REQUIRE_FALSE(ctx->loopAsync());
            REQUIRE(ctx->loopInterval() == 0);

            ctx->loopEnable(true);
            ctx->loopInterval(500);
            ctx->loopAsync(true);

            THEN("Should be set the loop parameters as well")
            {
                CHECK(ctx->loopEnable());
                CHECK(ctx->loopAsync());
                CHECK(ctx->loopInterval() == 500);
            }
        }

        WHEN("Call setup() method")
        {
            REQUIRE(info.str() == "");

            ctx->setup();
            THEN("The info console stream should capture 'setup' string")
            {
                CHECK(info.str() == "setup");
            }
        }

        WHEN("Call start() method")
        {
            REQUIRE(info.str() == "");

            ctx->start();
            THEN("The info console stream should capture 'start' string")
            {
                CHECK(info.str() == "start");
            }
        }

        WHEN("Call loop() method")
        {
            REQUIRE(info.str() == "");

            ctx->loop();
            THEN("The info console stream should capture 'loop' string")
            {
                CHECK(info.str() == "loop");
            }
        }

        WHEN("Call stop() method")
        {
            REQUIRE(info.str() == "");

            ctx->stop();
            THEN("The info console stream should capture 'stop' string")
            {
                CHECK(info.str() == "stop");
            }
        }
    }
}

SCENARIO("Test InstanceCtx of external module on normal usage", "[application][InstanceCtx]")
{
    std::ostringstream info;
    TestApp app([&info](std::ostringstream& msg) {info << msg.str(); });

    GIVEN("An external instance context")
    {
        app.factories.load("test", "ExternalModule");
        auto ctx = InstanceCtx::make("test", "test", app);

        WHEN("Call name() method")
        {
            auto name = ctx->name();
            THEN("Should return the instance name")
            {
                CHECK(name == "test");
            }
        }

        WHEN("Call setup() method")
        {
            REQUIRE(info.str() == "");

            ctx->setup();
            THEN("The info console stream should capture 'setup' string")
            {
                CHECK(info.str() == "setup");
            }
        }

        WHEN("Call start() method")
        {
            REQUIRE(info.str() == "");

            ctx->start();
            THEN("The info console stream should capture 'start' string")
            {
                CHECK(info.str() == "start");
            }
        }

        WHEN("Call loop() method")
        {
            REQUIRE(info.str() == "");

            ctx->loop();
            THEN("The info console stream should capture 'loop' string")
            {
                CHECK(info.str() == "loop");
            }
        }

        WHEN("Call stop() method")
        {
            REQUIRE(info.str() == "");

            ctx->stop();
            THEN("The info console stream should capture 'stop' string")
            {
                CHECK(info.str() == "stop");
            }
        }
    }
}
