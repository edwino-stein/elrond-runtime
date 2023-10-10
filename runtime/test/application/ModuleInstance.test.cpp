#include <elrond_mock.hpp>
#include <catch2.hpp>
#include "elrond_runtime.hpp"

#include "application/ModuleInstance.hpp"

using elrond::application::ModuleInstance;
using elrond::application::ModuleFactoryPool;
using elrond::mock::ConsoleAdapter;
using elrond::mock::Arguments;
using elrond::runtime::OStream;

class AppTest : public elrond::interface::Application, public elrond::mock::Application
{
    public:
        AppTest() = default;

        std::string name(elrond::interface::ModuleInstance&) const override
        { return "TEST"; }

        elrond::pointer<elrond::interface::Console> console(elrond::interface::ModuleInstance&) const override
        { return this->makeConsole(); }

        elrond::pointer<elrond::interface::Arguments> arguments(elrond::interface::ModuleInstance&) const override
        { return this->makeArguments(); }
};

class TestModule : public elrond::module::BaseGeneric
{
    public:
        void setup(elrond::Context ctx) override
        {
            ctx->console()->info() << ctx->name() << "->setup(): " << ctx->arguments()->asString("message");
            ctx->setLoopEvery(elrond::milliseconds(100));
        }

        void start(elrond::Context ctx) override { ctx->console()->info() << ctx->name() << "->start()"; }
        void loop(elrond::Context ctx) override { ctx->console()->info() << ctx->name() << "->loop()"; }
        void stop(elrond::Context ctx) override { ctx->console()->info() << ctx->name() << "->stop()"; }
};

SCENARIO("Test ModuleInstance on normal usage", "[application][ModuleInstance]")
{
    ModuleFactoryPool factories;
    AppTest app;
    std::ostringstream oss;
    ConsoleAdapter consoleAdapter([&oss](){ return std::make_shared<OStream>(oss); });
    Arguments args;

    factories.define<TestModule>("internal", {});
    factories.load("external", "misc/ExternalModule");
    args.set("message", "Some message here");
    app.with(consoleAdapter);
    app.with(args);

    GIVEN("An internal module instance")
    {
        ModuleInstance instance(app, *(factories.get("internal")));

        WHEN("Check loop default settings")
        {
            THEN("The loop settings should not be enabled and TS shold be 0s")
            {
                CHECK_FALSE(instance.isEnabledLoop());
                CHECK(instance.timespan().count == 0);
                CHECK(instance.timespan().unit == elrond::TimeUnit::SECONDS);
            }
        }

        WHEN("Check call setup method")
        {
            instance.setup();

            THEN("The console stream should hold the expected string")
            {
                CHECK(oss.str() == "TEST->setup(): Some message here");
            }
            
            AND_THEN("The loop settings should be enabled and TS shold be 100ms")
            {
                CHECK(instance.isEnabledLoop());
                CHECK(instance.timespan().count == 100);
                CHECK(instance.timespan().unit == elrond::TimeUnit::MILLISECONDS);
            }
        }

        WHEN("Check call start method")
        {
            instance.start();

            THEN("The console stream should hold the expected string")
            {
                CHECK(oss.str() == "TEST->start()");
            }
        }

        WHEN("Check call loop method")
        {
            instance.loop();

            THEN("The console stream should hold the expected string")
            {
                CHECK(oss.str() == "TEST->loop()");
            }
        }

        WHEN("Check call stop method")
        {
            instance.stop();

            THEN("The console stream should hold the expected string")
            {
                CHECK(oss.str() == "TEST->stop()");
            }
        }
    }

    GIVEN("An external module instance")
    {
        ModuleInstance instance(app, *(factories.get("external")));

        WHEN("Check loop default settings")
        {
            THEN("The loop settings should not be enabled and TS shold be 0s")
            {
                CHECK_FALSE(instance.isEnabledLoop());
                CHECK(instance.timespan().count == 0);
                CHECK(instance.timespan().unit == elrond::TimeUnit::SECONDS);
            }
        }

        WHEN("Check call setup method")
        {
            instance.setup();

            THEN("The console stream should hold the expected string")
            {
                CHECK(oss.str() == "setup");
            }
        }

        WHEN("Check call start method")
        {
            instance.start();

            THEN("The console stream should hold the expected string")
            {
                CHECK(oss.str() == "start");
            }
        }

        WHEN("Check call loop method")
        {
            instance.loop();

            THEN("The console stream should hold the expected string")
            {
                CHECK(oss.str() == "loop");
            }
        }

        WHEN("Check call stop method")
        {
            instance.stop();

            THEN("The console stream should hold the expected string")
            {
                CHECK(oss.str() == "stop");
            }
        }
    }
}
