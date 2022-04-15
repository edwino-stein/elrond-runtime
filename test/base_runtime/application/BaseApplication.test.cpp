#include <elrond_runtime.hpp>
#include <elrond_mock.hpp>
#include <catch2.hpp>

using elrond::application::BaseApplication;
using elrond::application::ModuleFactoryPool;
using elrond::mock::ConsoleAdapter;
using elrond::module::BaseGeneric;
using elrond::platform::ModuleInfo;

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


SCENARIO("Test the BaseApplication class in normal usage", "[application][BaseApplication]")
{
    GIVEN("An instance of BaseApplication")
    {
        std::ostringstream info;
        ConsoleAdapter adapter([&info](std::ostringstream& msg) { info << msg.str() << '\n'; });

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
                
                auto inst = app.get("test");
                CHECK(inst->name() == "test");
            }

            WHEN("Call run() method")
            {
                app.run();
                THEN("The instance lifecycle methods shold be called properly")
                {
                    CHECK(info.str() == "setup\nstart\nstop\n");
                }
            }
        }
    }
}
