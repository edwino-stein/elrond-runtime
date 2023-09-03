#include <elrond_mock.hpp>
#include <catch2.hpp>
#include "elrond_runtime.hpp"

using elrond::application::BaseApplication;
using elrond::application::ModuleFactoryPool;
using elrond::application::ModuleInstanceP;
using elrond::interface::ConsoleStreamAdapter;
using elrond::runtime::OStream;
using elrond::runtime::NullArguments;
using elrond::module::BaseGeneric;
using elrond::mock::Arguments;
using MConsoleAdapter = elrond::mock::ConsoleAdapter;
using IConsoleAdapter = elrond::interface::ConsoleAdapter;
using IModuleInstance = elrond::interface::ModuleInstance;
using Catch::Matchers::Contains;

class TestConsoleAdapter : public IConsoleAdapter, public MConsoleAdapter
{
    public:
        TestConsoleAdapter(): MConsoleAdapter(MConsoleAdapter::makeNullStream){}
        TestConsoleAdapter(MakeStreamH makeStream): MConsoleAdapter(makeStream){}

        elrond::pointer<ConsoleStreamAdapter> makeConsoleStreamAdapter(const std::string&) override
        { return MConsoleAdapter::makeConsoleStreamAdapter(); }
};

class TestModule : public BaseGeneric {};

SCENARIO("Test the BaseApplication object to check module instance creation", "[application][BaseApplication]")
{
    TestConsoleAdapter consoleAdapter;
    ModuleFactoryPool factories;
    factories.define<TestModule>("testInternal", {});
    factories.load("testExternal", "misc/ExternalModule");

    GIVEN("An instance of BaseApplication")
    {
        BaseApplication app(consoleAdapter);

        WHEN("Try to create an instance named 'test' from 'testInternal'")
        {
            CHECK_FALSE(app.exists("test"));

            THEN("An exception should be throwed")
            {
                try
                {
                    app.insert("test", *(factories.get("testInternal")));
                    CHECK(app.exists("test"));
                }
                catch(const std::exception& e)
                {
                    FAIL("Should not throw a exception");
                }
            }
        }

        WHEN("Try to create an instance named 'test' from 'testExternal'")
        {
            CHECK_FALSE(app.exists("test"));

            THEN("An exception should be throwed")
            {
                try
                {
                    app.insert("test", *(factories.get("testExternal")));
                    CHECK(app.exists("test"));
                }
                catch(const std::exception& e)
                {
                    FAIL("Should not throw a exception");
                }
            }
        }

        WHEN("Try to create an instance named 'test' twice")
        {
            CHECK_FALSE(app.exists("test"));
            app.insert("test", *(factories.get("testInternal")));
            CHECK(app.exists("test"));

            THEN("An exception should be throwed")
            {
                try
                {
                    app.insert("test", *(factories.get("testExternal")));
                    FAIL("Should throw a std::runtime_error");
                }
                catch(const std::exception& e)
                {
                    CHECK(isInstanceOf<std::runtime_error>(e));
                    CHECK(std::string(e.what()) == "Module instance redefinition");
                    CHECK(app.exists("test"));
                }
            }
        }
    }
}

SCENARIO("Test the BaseApplication object to check getters functions", "[application][BaseApplication]")
{
    std::ostringstream oss;
    TestConsoleAdapter consoleAdapter([&oss](){ return std::make_shared<OStream>(oss); });
    ModuleFactoryPool factories;
    factories.define<TestModule>("testInternal", {});
    factories.load("testExternal", "misc/ExternalModule");

    GIVEN("An instance of BaseApplication with two module instance")
    {
        BaseApplication app(consoleAdapter);
        auto& internal = app.insert("internal", *(factories.get("testInternal")));
        auto& external = app.insert("external", *(factories.get("testExternal")));

        WHEN("Call exists() method")
        {
            THEN ("The expected modules should exists")
            {
                CHECK(app.exists("internal"));
                CHECK(app.exists("external"));
                CHECK_FALSE(app.exists("undefined"));
            }
        }

        WHEN("Call get() method")
        {
            auto& internalPtr = app.get("internal");
            auto& externalPtr = app.get("external");

            THEN ("The returned module pointers should be the expecteds")
            {
                CHECK(&internalPtr == &internal);
                CHECK(&externalPtr == &external);
            }

            AND_THEN("Undefined instance should throw an exception")
            {
                try
                {
                    CHECK_FALSE(app.exists("undefined"));
                    app.get("undefined");
                    FAIL("No exception was thrown");
                }
                catch(const std::exception& e)
                {
                    CHECK(isInstanceOf<std::runtime_error>(e));
                    CHECK(std::string(e.what()) == "Invalid module instance");
                }
            }
        }

        WHEN("Call name() method")
        {
            auto internalName = app.name(internal);
            auto externalName = app.name(external);

            THEN ("The returned string names should be the expected")
            {
                CHECK(internalName == "internal");
                CHECK(externalName == "external");
            }
        }

        WHEN("Call console() method")
        {
            auto internalCls = app.console(internal);
            auto externalCls = app.console(external);

            THEN ("The internal instance console object should handle output as well")
            {
                internalCls->info("Internal message");
                CHECK(oss.str() == "Internal message");
            }

            AND_THEN ("The external instance console object should handle output as well")
            {
                externalCls->info("External message");
                CHECK(oss.str() == "External message");
            }
        }

        WHEN("Call arguments() method")
        {
            auto internalArgs = app.arguments(internal);
            auto externalArgs = app.arguments(external);

            THEN ("The returned argument objects should be instance of NullArguments")
            {
                CHECK(isInstanceOf<NullArguments>(internalArgs.get()));
                CHECK(isInstanceOf<NullArguments>(externalArgs.get()));
            }
        }

        WHEN("Call each() method")
        {
            std::vector<IModuleInstance*> itemPtr;
            std::vector<std::string> itemNames;
            app.each(
                [&itemPtr, &itemNames](IModuleInstance& inst, const std::string& name)
                {
                    itemPtr.push_back(&inst);
                    itemNames.push_back(name);
                }
            );
            
            THEN ("The returned objects should be instances and their names")
            {
                CHECK(itemPtr.size() == 2);
                CHECK(itemNames.size() == 2);

                CHECK_THAT(itemPtr, Contains(&internal));
                CHECK_THAT(itemNames, Contains("internal"));

                CHECK_THAT(itemPtr, Contains(&external));
                CHECK_THAT(itemNames, Contains("external"));
            }
        }
    }
}
