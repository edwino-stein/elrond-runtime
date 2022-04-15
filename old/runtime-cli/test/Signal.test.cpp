#include <elrond-test.hpp>
#include <elrond-catch.hpp>

#include "Signal.hpp"

TEST_CASE("Test Signal::attach, Signal::hasAttached and Signal::detach")
{
    CHECK_FALSE(Signal::hasAttached(SIG::INT));
    CHECK_FALSE(Signal::hasAttached(SIG::ABRT));

    Signal::attach(SIG::INT, [](){});
    CHECK(Signal::hasAttached(SIG::INT));

    Signal::attach(SIG::ABRT, [](){});
    CHECK(Signal::hasAttached(SIG::ABRT));

    Signal::detach(SIG::INT);
    Signal::detach(SIG::ABRT);

    CHECK_FALSE(Signal::hasAttached(SIG::INT));
    CHECK_FALSE(Signal::hasAttached(SIG::ABRT));
}

TEST_CASE("Test Signal::trigger")
{
    EXPECT_ASSERTS(3);

    Signal::attach(
        SIG::INT,
        [](){ CHECK_N_COUNT(((int) SIG::INT) == SIGINT); }
    );

    Signal::attach(
        SIG::ABRT,
        [](){ CHECK_N_COUNT(((int) SIG::ABRT) == SIGABRT); }
    );

    Signal::attach(
        SIG::SEGV,
        [](){ CHECK_N_COUNT(((int) SIG::SEGV) == SIGSEGV); }
    );

    Signal::trigger(SIG::INT);
    Signal::trigger(SIG::ABRT);
    Signal::trigger(SIG::SEGV);

    REQUIRE_ALL_DONE("Check if all tests are done");

    Signal::detach(SIG::INT);
    Signal::detach(SIG::ABRT);
    Signal::detach(SIG::SEGV);
}
