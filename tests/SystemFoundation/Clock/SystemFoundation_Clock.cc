#include <gtest/gtest.h>
#include <SystemFoundation/Clock/Clock.h>
#include <chrono>

using namespace std::chrono;
using namespace MIDILAR::SystemFoundation;

namespace MIDILAR::SystemFoundation{
    namespace{
        class SystemClockMock {
        public:
            time_point<system_clock> testStart; /**< Starting point for the mock clock. */
            Clock::TimePoint currentTime;       /**< The current simulated time. */
            Clock::Timebase frequency;          /**< The clock frequency in Hz. */

            /**
             * @brief Constructor initializes the mock clock.
             * Sets the starting time and default frequency.
             */
            SystemClockMock() 
                : testStart(system_clock::now()), currentTime(0), frequency(Clock::Seconds) {}

            /**
             * @brief Simulates the current time based on elapsed real time and clock frequency.
             * @return The simulated time as a `Clock::TimePoint`.
             */
            Clock::TimePoint getCurrentTime() {
                // Calculate elapsed time in nanoseconds since the last reset.
                auto elapsedNanos = duration_cast<nanoseconds>(system_clock::now() - testStart).count();
                // Convert nanoseconds to the desired timebase using the frequency.
                currentTime = static_cast<Clock::TimePoint>(elapsedNanos * (frequency / 1'000'000'000.0));
                return currentTime;
            }

            /**
             * @brief Sets the mock clock's frequency.
             * @param freq The frequency to set in Hz, using the `Clock::Timebase` enumeration.
             */
            void setFrequency(Clock::Timebase freq) {
                frequency = freq;
            }

            /**
             * @brief Resets the mock clock's starting point to the current time.
             */
            void reset() {
                testStart = system_clock::now();
            }
        };

        SystemClockMock MockCallbackHelper;

        Clock::TimePoint ClockCallback(){
            return MockCallbackHelper.getCurrentTime();
        }

        void SetupCallback(Clock::Timebase Freq){
            MockCallbackHelper.setFrequency(Freq);
        }

        class ClockTest : public testing::Test {
            private:

            protected:
            // You can do set-up work for each test here.
            ClockTest() {
            }

            // You can do clean-up work that doesn't throw exceptions here.
            ~ClockTest() override {
                
            }

            // If the constructor and destructor are not enough for setting up
            // and cleaning up each test, you can define the following methods:
            void SetUp() override {
                MockCallbackHelper.reset();
            }

            void TearDown() override {
                const ::testing::TestInfo* test_info = ::testing::UnitTest::GetInstance()->current_test_info();
                std::cout << "Test [" << test_info->test_suite_name() << "." << test_info->name() 
                        << "] Duration: " << MockCallbackHelper.getCurrentTime() << " ms" << std::endl;
            }

        };
    }
}

// Test default constructor
TEST(ClockTest, DefaultConstructor) {
    Clock clock;

    EXPECT_EQ(clock.getTime(), 0u);
    EXPECT_EQ(clock.getFrequency(), Clock::Timebase::Seconds);
    EXPECT_FALSE(clock.clockStatus());
}

// Test parameterized constructor
TEST(ClockTest, ParameterizedConstructor) {
    Clock clock(ClockCallback, 1000.0f);

    EXPECT_EQ(clock.getTime(), 0u);
    EXPECT_EQ(clock.getFrequency(), Clock::Timebase::Milliseconds);
    EXPECT_TRUE(clock.clockStatus());
}

// Test binding and unbinding the callback
TEST(ClockTest, CallbackBinding) {
    Clock clock;
    EXPECT_FALSE(clock.clockStatus());

    clock.bindClock(ClockCallback);
    EXPECT_TRUE(clock.clockStatus());

    clock.unbindClock();
    EXPECT_FALSE(clock.clockStatus());
}

// Test setting and getting the frequency
TEST(ClockTest, SetAndGetFrequency) {
    Clock clock;
    EXPECT_EQ(clock.setupStatus(), false);
    
    clock.bindSetup(SetupCallback);
    EXPECT_EQ(clock.setupStatus(), true);

    clock.setFrequency(Clock::Timebase::Milliseconds);
    EXPECT_EQ(clock.getFrequency(), Clock::Timebase::Milliseconds);

    clock.setFrequency(Clock::Timebase::Microseconds);
    EXPECT_EQ(clock.getFrequency(), Clock::Timebase::Microseconds);

    clock.setFrequency(Clock::Timebase::Seconds);
    EXPECT_EQ(clock.getFrequency(), Clock::Timebase::Seconds);
}

// Test refreshing the clock without a callback
TEST(ClockTest, RefreshWithoutCallback) {
    Clock clock;

    EXPECT_EQ(clock.refresh(), 0u);
    EXPECT_EQ(clock.refresh(), 0u);
}

// Test getting the current time
TEST(ClockTest, GetTime) {
    Clock clock(ClockCallback, Clock::Timebase::Milliseconds);

    auto t1 = clock.refresh();
    auto t2 = clock.refresh();
    EXPECT_GE(t2, t1);
}

// Test getting the current time
TEST(ClockTest, BindSetup) {
    Clock clock(ClockCallback, Clock::Timebase::Milliseconds);
    clock.bindSetup(SetupCallback);
    EXPECT_EQ(clock.getFrequency(), Clock::Timebase::Milliseconds);
    clock.setFrequency(Clock::Timebase::Microseconds);
    EXPECT_EQ(clock.getFrequency(), Clock::Timebase::Microseconds);
    clock.unbindSetup();
    clock.setFrequency(Clock::Timebase::Milliseconds);
    EXPECT_EQ(clock.getFrequency(), Clock::Timebase::Microseconds);
}

// Test getting the current time
TEST(ClockTest, setPeriod) {
    Clock clock(ClockCallback, Clock::Timebase::Milliseconds);
    clock.bindSetup(SetupCallback);
    EXPECT_EQ(clock.getFrequency(), Clock::Timebase::Milliseconds);
    clock.setPeriod(0.000001);
    EXPECT_EQ(clock.getFrequency(), Clock::Timebase::Microseconds);
}