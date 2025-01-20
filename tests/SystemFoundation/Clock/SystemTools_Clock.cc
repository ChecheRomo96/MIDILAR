#include <gtest/gtest.h>
#include <SystemFoundation/Clock.h>

using namespace MIDILAR::SystemFoundation;

// A mock callback class for testing
class MockCallback {
public:
    static Clock::TimePoint time;

    static Clock::TimePoint callback() {
        return ++time; // Increment time on each call
    }

    static void reset() {
        time = 0; // Reset the counter
    }
};

// Initialize static member
Clock::TimePoint MockCallback::time = 0;

// Test default constructor
TEST(ClockTest, DefaultConstructor) {
    MockCallback::reset(); // Reset the counter before test
    Clock clock;

    EXPECT_EQ(clock.getTime(), 0u);
    EXPECT_EQ(clock.getFrequency(), Clock::Timebase::Seconds);
    EXPECT_FALSE(clock.callbackStatus());
}

// Test parameterized constructor
TEST(ClockTest, ParameterizedConstructor) {
    MockCallback::reset(); // Reset the counter before test
    Clock clock(MockCallback::callback, 1000.0f);

    EXPECT_EQ(clock.getTime(), 0u);
    EXPECT_EQ(clock.getFrequency(), Clock::Timebase::Milliseconds);
    EXPECT_TRUE(clock.callbackStatus());
}

// Test binding and unbinding the callback
TEST(ClockTest, CallbackBinding) {
    MockCallback::reset(); // Reset the counter before test
    Clock clock;

    EXPECT_FALSE(clock.callbackStatus());

    clock.bindCallback(MockCallback::callback);
    EXPECT_TRUE(clock.callbackStatus());

    clock.unbindCallback();
    EXPECT_FALSE(clock.callbackStatus());
}

// Test setting and getting the frequency
TEST(ClockTest, SetAndGetFrequency) {
    MockCallback::reset(); // Reset the counter before test
    Clock clock;

    clock.setFrequency(Clock::Timebase::Milliseconds);
    EXPECT_EQ(clock.getFrequency(), Clock::Timebase::Milliseconds);

    clock.setFrequency(Clock::Timebase::Microseconds);
    EXPECT_EQ(clock.getFrequency(), Clock::Timebase::Microseconds);

    clock.setFrequency(Clock::Timebase::Seconds);
    EXPECT_EQ(clock.getFrequency(), Clock::Timebase::Seconds);
}

// Test refreshing the clock with a bound callback
TEST(ClockTest, RefreshWithCallback) {
    MockCallback::reset(); // Reset the counter before test
    Clock clock(MockCallback::callback, Clock::Timebase::Milliseconds);

    EXPECT_EQ(clock.refresh(), 1u);
    EXPECT_EQ(clock.refresh(), 2u);
    EXPECT_EQ(clock.refresh(), 3u);
}

// Test refreshing the clock without a callback
TEST(ClockTest, RefreshWithoutCallback) {
    MockCallback::reset(); // Reset the counter before test
    Clock clock;

    EXPECT_EQ(clock.refresh(), 0u);
    EXPECT_EQ(clock.refresh(), 0u);
}

// Test getting the current time
TEST(ClockTest, GetTime) {
    MockCallback::reset(); // Reset the counter before test
    Clock clock(MockCallback::callback, Clock::Timebase::Milliseconds);

    clock.refresh();
    EXPECT_EQ(clock.getTime(), 1u);

    clock.refresh();
    EXPECT_EQ(clock.getTime(), 2u);
}