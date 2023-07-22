#include <elrond_mock.hpp>
#include <catch2.hpp>
#include "elrond_runtime.hpp"

using elrond::application::AsyncLoopTask;
using elrond::interface::LoopTask;

class TestLoopTask: public LoopTask
{
    protected:
        elrond::procedure<> _task;
        elrond::TimeSpan _timespan;

    public:

        TestLoopTask(elrond::procedure<> task, elrond::TimeSpan timespan)
        : _task(task), _timespan(timespan) {}

        void loop() { this->_task(); }

        elrond::TimeSpan timespan() const override
        { return this->_timespan; }
};

SCENARIO("Test AsyncLoopTask states", "[application][AsyncLoopTask]")
{
    GIVEN("A task that do nothing and execute every 1s")
    {
        TestLoopTask task([](){}, elrond::milliseconds(500));
        AsyncLoopTask alt(task);

        WHEN("Check the initial status")
        {
            THEN("The taks should be not running and not stopping")
            {
                CHECK_FALSE(alt.isRunning());
                CHECK_FALSE(alt.isStopping());
                CHECK_FALSE(alt.wasDetached());
            }
        }

        WHEN("Start the task")
        {
            alt.run();

            THEN("The task state should not change right before starts")
            {
                CHECK_FALSE(alt.isRunning());
                CHECK_FALSE(alt.isStopping());
                CHECK_FALSE(alt.wasDetached());
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            AND_THEN("The task state only will change to running after few milliseconds")
            {
                CHECK(alt.isRunning());
                CHECK_FALSE(alt.isStopping());
                CHECK_FALSE(alt.wasDetached());
            }

            AND_WHEN("Wait for 500ms and stop the task")
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                alt.stop();

                THEN("The task state should change to stopping, but the task will not stop immediately")
                {
                    CHECK_FALSE(alt.isRunning());
                    CHECK(alt.isStopping());
                    CHECK_FALSE(alt.wasDetached());
                }
            }

            AND_WHEN("Wait for 500ms, stop the task and wait for more 500ms to be sure that task was really finished")
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                alt.stop();
                std::this_thread::sleep_for(std::chrono::milliseconds(500));

                THEN("The task state now should be completely stopped and not must be detached")
                {
                    CHECK_FALSE(alt.isRunning());
                    CHECK_FALSE(alt.isStopping());
                    CHECK_FALSE(alt.wasDetached());
                }
            }

            AND_WHEN("Wait for 100ms and force the task to stop and finish")
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                alt.stop();
                alt.finish(true);

                THEN("The task state now should be completely stopped and must be detached")
                {
                    CHECK_FALSE(alt.isRunning());
                    CHECK_FALSE(alt.isStopping());
                    CHECK(alt.wasDetached());
                }
            }

            AND_WHEN("The task object was destryed before it was stopped and finished")
            {
                alt.~AsyncLoopTask();

                THEN("The task state now should be completely stopped and must be detached")
                {
                    CHECK_FALSE(alt.isRunning());
                    CHECK_FALSE(alt.isStopping());
                    CHECK(alt.wasDetached());
                }
            }

            // Guarantee that all test thread tasks was finished
            alt.finish(false);
        }
    }
}

SCENARIO("Test AsyncLoopTask exception handling", "[application][AsyncLoopTask]")
{
    GIVEN("A task that throw an exception given a some condidion")
    {
        bool error = false;
        TestLoopTask task(
            [&error](){ if(error) throw std::runtime_error("It is an error"); },
            elrond::milliseconds(0)
        );
        AsyncLoopTask alt(task);

        WHEN("Start the task")
        {
            alt.run();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            THEN("The task state only will change to running after few milliseconds")
            {
                REQUIRE(alt.isRunning());
                REQUIRE_FALSE(alt.isStopping());
                CHECK_FALSE(alt.wasDetached());
            }

            AND_WHEN("An exception was throwed during task execution")
            {
                error = true;
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                THEN("The task state should change to stopped and no exceptions was propagated yet to parent thread")
                {
                    CHECK_FALSE(alt.isRunning());
                    CHECK_FALSE(alt.isStopping());
                    CHECK_FALSE(alt.wasDetached());
                }

                AND_WHEN("The task object was destroyed before it finished")
                {
                    try
                    {
                        alt.~AsyncLoopTask();
                        THEN("No exceptions should be propagated to parent thread")
                        {
                            SUCCEED();
                        }
                    }
                    catch(...)
                    {
                        THEN("No exceptions should be propagated to parent thread")
                        {
                            FAIL();
                        }
                    }
                }

                AND_WHEN("The task was finished before it destruction")
                {
                    try
                    {
                        alt.finish(false);
                        THEN("The exception should be propagated to parent thread")
                        {
                            FAIL();
                        }
                    }
                    catch(const std::exception& e)
                    {
                        THEN("The exception should be propagated to parent thread")
                        {
                            CHECK(std::string(e.what()) == "It is an error");
                        }
                    }
                }
            }

            // Guarantee that all test thread tasks was finished
            try { alt.finish(false); } catch(...) {}
        }
    }
}
