#include <elrond_mock.hpp>
#include <catch2.hpp>
#include "elrond_runtime.hpp"

using elrond::application::RuntimeApplication;
using elrond::module::BaseGeneric;
using elrond::application::ModuleFactoryPool;
using elrond::interface::ConsoleStreamAdapter;
using elrond::runtime::OStream;
using MConsoleAdapter = elrond::mock::ConsoleAdapter;
using IConsoleAdapter = elrond::interface::ConsoleAdapter;
using IModuleInstance = elrond::interface::ModuleInstance;
using IFactoryAdapter = elrond::interface::FactoryAdapter;
using State = elrond::application::State;
using Catch::Matchers::Equals;

struct TestAppEvents
{
    elrond::procedure<State> onStateChanged = [](State){};
    elrond::procedure<const std::string&, const std::string&> onInstanceCreating = [](const std::string&, const std::string&){};
    elrond::procedure<IModuleInstance&, IFactoryAdapter&> onInstanceCreated = [](IModuleInstance&, IFactoryAdapter&){};
    elrond::procedure<IModuleInstance&> onInstanceSetup = [](IModuleInstance&){};
    elrond::procedure<IModuleInstance&> onInstanceStart = [](IModuleInstance&){};
    elrond::procedure<IModuleInstance&> onInstanceStop = [](IModuleInstance&){};
};

class TestApp : public RuntimeApplication {
    public:
        TestAppEvents events;

        TestApp(ModuleFactoryPool& factoies, IConsoleAdapter& consoleAdapter):
        RuntimeApplication(factoies, consoleAdapter), events() {}

        void onStateChanged(State prev) const override
        { this->events.onStateChanged(prev); }

        void onInstanceCreating(const std::string& name, const std::string& factory) const override
        { this->events.onInstanceCreating(name, factory); }

        void onInstanceCreated(IModuleInstance& instance, IFactoryAdapter& factory) const override
        { this->events.onInstanceCreated(instance, factory); }
    
        void onInstanceSetup(IModuleInstance& instance) const override
        { this->events.onInstanceSetup(instance); }

        void onInstanceStart(IModuleInstance& instance) const override
        { this->events.onInstanceStart(instance); }

        void onInstanceStop(IModuleInstance& instance) const override
        { this->events.onInstanceStop(instance); }
};

class TestConsoleAdapter : public IConsoleAdapter, public MConsoleAdapter
{
    public:
        TestConsoleAdapter(): MConsoleAdapter(MConsoleAdapter::makeNullStream){}
        TestConsoleAdapter(MakeStreamH makeStream, AppendH preAppend, AppendH postAppend):
        MConsoleAdapter(makeStream, preAppend, postAppend){}

        elrond::pointer<ConsoleStreamAdapter> makeConsoleStreamAdapter(const std::string&) override
        { return MConsoleAdapter::makeConsoleStreamAdapter(); }
};

class TestModule : public BaseGeneric
{
    public:
        void setup(elrond::Context ctx) override { ctx->console()->info("setup"); }
        void start(elrond::Context ctx) override { ctx->console()->info("start"); }
        void loop(elrond::Context ctx) override { ctx->console()->info("loop"); }
        void stop(elrond::Context ctx) override { ctx->console()->info("stop"); }
};

SCENARIO("Test the BaseApplication object to check events and state", "[application][BaseApplication]")
{
    std::ostringstream oss;
    TestConsoleAdapter consoleAdapter;
    ModuleFactoryPool factories;

    GIVEN("An instance of BaseApplication with an internal module factory")
    {
        factories.define<TestModule>("testInternal", {});
        TestApp app(factories, consoleAdapter);

        WHEN("Try to create an instance of 'test' of 'testInternal'")
        {
            std::vector<std::string> eventOrder;
            std::vector<std::function<void()>> asserts;

            app.events.onInstanceCreating = [&eventOrder, &asserts](const std::string& name, const std::string& factory)
            {
                eventOrder.push_back("onInstanceCreating");
                asserts.push_back([=](){ CHECK(name == "test"); });
                asserts.push_back([=](){ CHECK(factory == "testInternal"); });
            };

            app.events.onInstanceCreated = [&eventOrder, &asserts, &app, &factories](IModuleInstance& instance, IFactoryAdapter& factory)
            {
                eventOrder.push_back("onInstanceCreated");
                asserts.push_back([&](){ CHECK(app.name(instance) == "test"); CHECK(app.name(instance) == "test"); });
                asserts.push_back([&](){ CHECK(factories.get("testInternal").get() == &factory); });
            };

            CHECK_FALSE(app.exists("test"));
            app.create("test", "testInternal");

            THEN("The instance should be created")
            {
                CHECK(app.exists("test"));
                CHECK(eventOrder.size() == 2);
                CHECK_THAT(eventOrder, Equals(std::vector<std::string>({"onInstanceCreating", "onInstanceCreated"})));
                for (auto a : asserts) a();
            }
        }

        WHEN("Try to setup, start and run per 100ms, and then stop normally")
        {
            std::vector<std::string> eventOrder;
            std::vector<std::function<void()>> asserts;

            app.events.onStateChanged = [&app, &eventOrder, &asserts](State prev)
            {
                State current = app.state();
                std::ostringstream oss;
                oss << "onStateChanged(" << static_cast<int>(prev) << "->" << static_cast<int>(current) << ")";
                eventOrder.push_back(oss.str());

                switch (prev)
                {
                    case State::CREATED:
                        asserts.push_back([current](){ CHECK(current == State::INITIALIZING); });
                    break;
                    case State::INITIALIZING:
                        asserts.push_back([current](){ CHECK(current == State::INITIALIZED); });
                    break;
                    case State::INITIALIZED:
                        asserts.push_back([current](){ CHECK(current == State::STARTING); });
                    break;
                    case State::STARTING:
                        asserts.push_back([current](){ CHECK(current == State::RUNNING); });
                    break;
                    case State::RUNNING:
                        asserts.push_back([current](){ CHECK(current == State::RUNNING_AND_STOPPING); });
                    break;
                    case State::RUNNING_AND_STOPPING:
                        asserts.push_back([current](){ CHECK(current == State::STOPPING); });
                    break;
                    case State::STOPPING:
                        asserts.push_back([current](){ CHECK(current == State::STOPPED); });
                    break;
                    default:
                        asserts.push_back([](){ FAIL("Invalid app state"); });
                }
            };

            app.create("test", "testInternal");

            try
            {
                app.setup();
                auto f = app.start();

                std::this_thread::sleep_for(std::chrono::milliseconds(100));

                app.stop();
                f.wait();
                f.get();
            }
            catch(const std::exception&)
            {
                FAIL("Unexpected exception");
            }

            THEN("The events should occurs as expected")
            {
                CHECK(eventOrder.size() == 7);
                CHECK_THAT(
                    eventOrder,
                    Equals(std::vector<std::string>({
                        "onStateChanged(0->1)", "onStateChanged(1->2)", "onStateChanged(2->3)", "onStateChanged(3->4)",
                        "onStateChanged(4->5)", "onStateChanged(5->6)", "onStateChanged(6->8)"
                    }))
                );
                for (auto a : asserts) a();
            }
        }
    }
}
