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
    TestApp app([&info](std::ostringstream& msg) {info << msg.str() << std::endl;});

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
                CHECK(isInstanceOf<Console>(console.get()));
            }
        }

        WHEN("Call arguments() method")
        {
            auto args = ctx->arguments();
            THEN("Should return the arguments instance")
            {
                CHECK(isInstanceOf<NullArguments>(args.get()));
            }
        }

        WHEN("Set loop parameters")
        {
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
            ctx->setup();
            THEN("The info console stream should capture 'setup' string")
            {
                CHECK_THAT(info.str(), Contains("setup"));
            }
        }

        WHEN("Call start() method with loop disabled")
        {
            ctx->loopEnable(false);
            auto f = ctx->start();

            THEN("Should return a future with status deferred")
            {
                CHECK(f.wait_for(std::chrono::milliseconds(0)) == std::future_status::deferred);
                auto cfg = f.get();

                AND_THEN("The InstanceLoopCfg should define the instance loop as disabled")
                {
                    CHECK_FALSE(cfg.enabled);
                }

                AND_THEN("The info console stream should capture 'start' string")
                {
                    CHECK_THAT(info.str(), Contains("start"));
                }
            }
        }

        WHEN("Call start() method with loop enabled for every 100ms")
        {
            ctx->loopEnable(true);
            ctx->loopAsync(true);
            ctx->loopInterval(100);

            auto f = ctx->start();

            THEN("Should return a future with status timeout")
            {
                CHECK(f.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout);
                auto cfg = f.get();

                AND_THEN("The InstanceLoopCfg should define the instance loop as enabled")
                {
                    CHECK(cfg.enabled);
                }

                AND_THEN("The InstanceLoopCfg should define the instance next loop to 100ms")
                {
                    CHECK(cfg.interval == 100);
                }

                AND_THEN("The info console stream should capture 'loop' string")
                {
                    CHECK_THAT(info.str(), Contains("loop"));
                }
            }
        }

        WHEN("Call setup() method")
        {
            ctx->stop();
            THEN("The info console stream should capture 'stop' string")
            {
                CHECK_THAT(info.str(), Contains("stop"));
            }
        }
    }
}

SCENARIO("Test InstanceCtx of external module on normal usage", "[application][InstanceCtx]")
{
    std::ostringstream info;
    TestApp app([&info](std::ostringstream& msg) {info << msg.str() << std::endl;});

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
                CHECK_THAT(info.str(), Contains("setup"));
            }
        }

        WHEN("Call start() method with loop disabled")
        {
            ctx->loopEnable(false);
            auto f = ctx->start();

            THEN("Should return a future with status deferred")
            {
                CHECK(f.wait_for(std::chrono::milliseconds(0)) == std::future_status::deferred);
                auto cfg = f.get();

                AND_THEN("The InstanceLoopCfg should define the instance loop as disabled")
                {
                    CHECK_FALSE(cfg.enabled);
                }

                AND_THEN("The info console stream should capture 'start' string")
                {
                    CHECK_THAT(info.str(), Contains("start"));
                }
            }
        }

        WHEN("Call start() method with loop enabled for every 100ms")
        {
            ctx->loopEnable(true);
            ctx->loopAsync(true);
            ctx->loopInterval(100);

            auto f = ctx->start();

            THEN("Should return a future with status timeout")
            {
                CHECK(f.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout);
                auto cfg = f.get();

                AND_THEN("The InstanceLoopCfg should define the instance loop as enabled")
                {
                    CHECK(cfg.enabled);
                }

                AND_THEN("The InstanceLoopCfg should define the instance next loop to 100ms")
                {
                    CHECK(cfg.interval == 100);
                }

                AND_THEN("The info console stream should capture 'loop' string")
                {
                    CHECK_THAT(info.str(), Contains("loop"));
                }
            }
        }

        WHEN("Call setup() method")
        {
            ctx->stop();
            THEN("The info console stream should capture 'stop' string")
            {
                CHECK_THAT(info.str(), Contains("stop"));
            }
        }
    }
}
