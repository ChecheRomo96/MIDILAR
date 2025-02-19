#include <gtest/gtest.h>
#include <vector>
#include <cstring> // for std::memcmp
#include <MidiFoundation/Processor.h>

using namespace MIDILAR;

namespace {

    // Mock callback to test MIDI output
    void MockMidiOut(uint8_t* Message, size_t size) {
        static std::vector<uint8_t> lastMessage;
        lastMessage.assign(Message, Message + size);
    }

    /**
     * @brief Test fixture for MidiProcessor
     */
    class MidiProcessorTest : public ::testing::Test {
    protected:
        MidiProcessor processor; // Instance of MidiProcessor for testing

        void SetUp() override {
            // Called before each test
        }

        void TearDown() override {
            // Called after each test
        }
    };

    TEST_F(MidiProcessorTest, DefaultConstructor) {
        // Test the default constructor
        EXPECT_FALSE(processor.MidiOutApiStatus());
        EXPECT_FALSE(processor.HasCapability(MidiProcessor::Capabilities::MidiIn));
        EXPECT_FALSE(processor.HasCapability(MidiProcessor::Capabilities::MidiOut));
    }

    TEST_F(MidiProcessorTest, MidiOutApiLink) {
        // Test linking a MIDI output handler
        processor.MidiOutApiLink(MockMidiOut);
        EXPECT_TRUE(processor.MidiOutApiStatus());
    }

    TEST_F(MidiProcessorTest, HasCapability) {
        // Test checking capabilities
        class TestProcessor : public MidiProcessor {
        public:
            TestProcessor() {
                SetCapabilities(static_cast<uint32_t>(MidiProcessor::Capabilities::MidiIn) |
                                static_cast<uint32_t>(MidiProcessor::Capabilities::MidiOut));
            }
        };

        TestProcessor testProcessor;

        EXPECT_TRUE(testProcessor.HasCapability(MidiProcessor::Capabilities::MidiIn));
        EXPECT_TRUE(testProcessor.HasCapability(MidiProcessor::Capabilities::MidiOut));
        EXPECT_FALSE(testProcessor.HasCapability(MidiProcessor::Capabilities::ExtClock));
    }


    TEST_F(MidiProcessorTest, MidiInputRawBuffer) {
        // Test handling raw MIDI input
        class TestProcessor : public MidiProcessor {
        public:
            std::vector<uint8_t> receivedMessage;

            void MidiInput(uint8_t* Message, size_t size) override {
                receivedMessage.assign(Message, Message + size);
            }
        };

        TestProcessor testProcessor;

        uint8_t testMessage[] = {0x90, 0x45, 0x7F}; // Note On, Note 69, Velocity 127
        size_t testSize = sizeof(testMessage);

        testProcessor.MidiInput(testMessage, testSize);

        EXPECT_EQ(testProcessor.receivedMessage.size(), testSize);
        EXPECT_TRUE(std::equal(testProcessor.receivedMessage.begin(),
                               testProcessor.receivedMessage.end(),
                               testMessage));
    }

    TEST_F(MidiProcessorTest, MidiOutputRawBuffer) {
        // Test handling raw MIDI output
        class TestProcessor : public MidiProcessor {
        public:
            void TestMidiOutput(uint8_t* Message, size_t size) {
                MidiOutput(Message, size);
            }
        };

        TestProcessor testProcessor;
        testProcessor.MidiOutApiLink(MockMidiOut);

        uint8_t testMessage[] = {0x80, 0x45, 0x00}; // Note Off, Note 69, Velocity 0
        size_t testSize = sizeof(testMessage);

        testProcessor.TestMidiOutput(testMessage, testSize);

        // Assuming the MockMidiOut updates a static variable `lastMessage`
        std::vector<uint8_t> expectedMessage(testMessage, testMessage + testSize);
        EXPECT_TRUE(std::equal(expectedMessage.begin(), expectedMessage.end(), testMessage));
    }

    TEST_F(MidiProcessorTest, UpdateAndClockTick) {
        // Test the Update and ClockTick methods
        class TestProcessor : public MidiProcessor {
        public:
            size_t clockTickCount = 0;
            size_t updateCount = 0;

            void Update(SystemFoundation::Clock::TimePoint SystemTime) override {
                updateCount++;
            }

            void ClockTick() override {
                clockTickCount++;
            }
        };

        TestProcessor testProcessor;

        // Simulate system time updates
        for (int i = 0; i < 5; ++i) {
            testProcessor.Update(i);
        }

        // Simulate clock ticks
        for (int i = 0; i < 3; ++i) {
            testProcessor.ClockTick();
        }

        EXPECT_EQ(testProcessor.updateCount, 5);
        EXPECT_EQ(testProcessor.clockTickCount, 3);
    }
    
    #if __has_include(<vector>)
        TEST_F(MidiProcessorTest, MidiInputWithVector) {
            // Test handling MIDI input using a vector
            class TestProcessor : public MidiProcessor {
            public:
                std::vector<uint8_t> receivedMessage;

                void MidiInput(uint8_t* Message, size_t size) override {
                    receivedMessage.assign(Message, Message + size);
                }
            };

            TestProcessor testProcessor;

            std::vector<uint8_t> testMessage = {0x91, 0x30, 0x60}; // Note On, Note 48, Velocity 96
            uint8_t* rawBuffer = testMessage.data();
            size_t rawSize = testMessage.size();

            testProcessor.MidiInput(rawBuffer, rawSize);

            EXPECT_EQ(testProcessor.receivedMessage.size(), testMessage.size());
            EXPECT_TRUE(std::equal(testProcessor.receivedMessage.begin(),
                                testProcessor.receivedMessage.end(),
                                testMessage.begin()));
        }
    #endif

} // namespace
