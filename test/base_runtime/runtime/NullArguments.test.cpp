#include <runtime/NullArguments.hpp>
#include "catch2.hpp"

using elrond::runtime::NullArguments;

TEST_CASE("Check NullArguments 'existis' method", "[runtime][NullArguments]")
{
    NullArguments args;
    CHECK_FALSE(args.exists("key"));
}

TEST_CASE("Check NullArguments 'is' methods", "[runtime][NullArguments]")
{
    NullArguments args;
    CHECK_FALSE(args.isInt("key"));
    CHECK_FALSE(args.isUInt("key"));
    CHECK_FALSE(args.isBool("key"));
    CHECK_FALSE(args.isDouble("key"));
    CHECK_FALSE(args.isString("key"));
}

TEST_CASE("Check NullArguments 'as' methods", "[runtime][NullArguments]")
{
    NullArguments args;
    CHECK(args.asInt("key") == elrond::int32());
    CHECK(args.asUInt("key") == elrond::uInt32());
    CHECK(args.asBool("key") == false);
    CHECK(args.asDouble("key") == double());
    CHECK(args.asString("key") == "");

    char buffer[5] = "Test";
    CHECK(args.asString("key", buffer, 5) == 0);
    CHECK(std::string(buffer) == "Test");
}
