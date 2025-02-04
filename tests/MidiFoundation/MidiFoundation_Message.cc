#include <gtest/gtest.h>
#include <MidiFoundation/Message/Message.h>
#include <MidiFoundation/Protocol/Defines.h>
#include <MidiFoundation/Protocol/Enums.h>
#include <vector>
#include <cstdint>

namespace MIDILAR::MidiFoundation {

    namespace {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Midi Message GTest class 

        class MidiMessageTest : public testing::Test {
        protected:
            // You can do set-up work for each test here.
            MidiMessageTest() {}

            // You can do clean-up work that doesn’t throw exceptions here.
            ~MidiMessageTest() override {}

            // If the constructor and destructor are not enough for setting up
            // and cleaning up each test, you can define the following methods:
            void SetUp() override {}

            void TearDown() override {}
        };
    //
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Utility function to clamp a value to a maximum

        inline int Clamp(int value, int min, int max) {
            if (value < min) return min;
            if (value > max) return max;
            return value;
        }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Constructor Tests

        TEST(MidiMessageTest, DefaultConstructor) {
            Message msg;
            EXPECT_EQ(msg.size(), 0);
            EXPECT_EQ(msg.Buffer(), nullptr);
        }

        TEST(MidiMessageTest, CopyConstructor) {
            Message original;
            original.NoteOn(60, 100, 1);  // Create a NoteOn message

            Message copy(original);
            EXPECT_EQ(copy.size(), original.size());
            EXPECT_EQ(memcmp(copy.Buffer(), original.Buffer(), original.size()), 0);
        }

        TEST(MidiMessageTest, MoveConstructor) {
            Message original;
            original.NoteOn(60, 100, 1);  // Create a NoteOn message

            Message moved(std::move(original));
            EXPECT_EQ(moved.size(), 3);
            EXPECT_EQ(moved.Data(0), 0x91);
            EXPECT_EQ(moved.Data(1), 60);
            EXPECT_EQ(moved.Data(2), 100);
        }

        TEST(MidiMessageTest, RawBufferConstructor) {
            uint8_t buffer[] = {0x90, 60, 100};
            Message msg(buffer, sizeof(buffer));

            EXPECT_EQ(msg.size(), 3);
            EXPECT_EQ(msg.Data(0), 0x90);
            EXPECT_EQ(msg.Data(1), 60);
            EXPECT_EQ(msg.Data(2), 100);
        }

        #if __has_include(<vector>)
        TEST(MidiMessageTest, VectorConstructor) {
            std::vector<uint8_t> data = {0x90, 60, 100};
            Message msg(data);

            EXPECT_EQ(msg.size(), 3);
            EXPECT_EQ(msg.Vector(), data);
        }

        TEST(MidiMessageTest, MoveVectorConstructor) {
            std::vector<uint8_t> data = {0x90, 60, 100};
            Message msg(std::move(data));

            EXPECT_EQ(msg.size(), 3);
            EXPECT_EQ(msg.Data(0), 0x90);
            EXPECT_EQ(msg.Data(1), 60);
            EXPECT_EQ(msg.Data(2), 100);
        }
        #endif
    //
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Data Access Tests

        TEST(MidiMessageTest, BufferAccess) {
            Message msg;
            msg.NoteOn(60, 100, 1);

            EXPECT_NE(msg.Buffer(), nullptr);
            EXPECT_EQ(msg.Data(0), 0x91);
            EXPECT_EQ(msg.Data(1), 60);
            EXPECT_EQ(msg.Data(2), 100);
        }

        TEST(MidiMessageTest, SizeAccess) {
            Message msg;
            EXPECT_EQ(msg.size(), 0);

            msg.NoteOn(60, 100, 1);
            EXPECT_EQ(msg.size(), 3);
        }

        #if __has_include(<vector>)
        TEST(MidiMessageTest, VectorAccess) {
            Message msg;
            msg.NoteOn(60, 100, 1);

            std::vector<uint8_t> expected = {0x91, 60, 100};
            EXPECT_EQ(msg.Vector(), expected);
        }
        #endif
    //
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Vector API Tests

        TEST(MidiMessageTest, VectorReturnsCorrectBuffer) {
            #if __has_include(<vector>)
            // Arrange
            std::vector<uint8_t> expectedBuffer = {0x90, 0x45, 0x7F}; // Example MIDI Note On message.
            Message message(expectedBuffer);

            // Act
            const std::vector<uint8_t>& actualBuffer = message.Vector();

            // Assert
            EXPECT_EQ(actualBuffer.size(), expectedBuffer.size()) << "The vector size does not match.";
            for (size_t i = 0; i < expectedBuffer.size(); ++i) {
                EXPECT_EQ(actualBuffer[i], expectedBuffer[i]) << "Buffer contents mismatch at index " << i;
            }
            #endif
        }

        TEST(MidiMessageTest, BeginAndEndIteratorsNonConst) {
            #if __has_include(<vector>)
            // Arrange
            std::vector<uint8_t> expectedBuffer = {0x90, 0x45, 0x7F};
            Message message(expectedBuffer);

            // Act
            auto it = message.begin();
            *it = 0x80; // Modify the first byte of the buffer.

            // Assert
            EXPECT_EQ(*message.begin(), 0x80);
            #endif
        }

        TEST(MidiMessageTest, BeginAndEndIteratorsConst) {
            #if __has_include(<vector>)
            // Arrange
            const std::vector<uint8_t> expectedBuffer = {0x90, 0x45, 0x7F};
            Message message(expectedBuffer);

            // Act
            auto it = message.begin();
            auto end = message.end();

            // Assert
            ASSERT_EQ(std::distance(it, end), 3);
            EXPECT_EQ(*it, 0x90);
            #endif
        }

        TEST(MidiMessageTest, ModifyingVectorThroughIterators) {
            #if __has_include(<vector>)
            // Arrange
            std::vector<uint8_t> expectedBuffer = {0x90, 0x45, 0x7F};
            Message message(expectedBuffer);

            // Act
            auto it = message.begin();
            *it = 0x80; // Modify the first byte.

            // Assert
            EXPECT_EQ(message.Vector()[0], 0x80);
            #endif
        }

        TEST(MidiMessageTest, EmptyVector) {
        #if __has_include(<vector>)
        // Arrange
        std::vector<uint8_t> emptyBuffer;
        Message message(emptyBuffer);

        // Act
        auto it = message.begin();
        auto end = message.end();

        // Assert
        EXPECT_EQ(std::distance(it, end), 0);
        #endif
    }
    //
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Message Construction Tests for Channel Voice Messages

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Note On

            // Utility function to validate a Note On message
            void ValidateNoteOnMessage(Message& msg, uint8_t channel, uint8_t pitch, uint8_t velocity) {
                // Clamp values to their valid MIDI ranges
                uint8_t expectedChannel = Clamp(channel, 0, 15);    // MIDI channels are 0–15
                uint8_t expectedPitch = Clamp(pitch, 0, 127);       // MIDI pitches are 0–127
                uint8_t expectedVelocity = Clamp(velocity, 0, 127); // MIDI velocities are 0–127

                // Verify the message structure
                EXPECT_EQ(msg.size(), 3); // Note On messages are 3 bytes
                EXPECT_EQ(msg.Data(0), MIDI_NOTE_ON + expectedChannel); // Command + Channel
                EXPECT_EQ(msg.Data(1), expectedPitch);                  // Pitch
                EXPECT_EQ(msg.Data(2), expectedVelocity);               // Velocity
            }

            // NoteOnMessage Test
            TEST(MidiMessageTest, NoteOnMessage) {
            Message msg;
            
            // Define a set of test cases for channel, pitch, and velocity
            std::vector<uint8_t> testChannels = {0, 7, 15, 16, 255}; // Valid and out-of-bounds
            std::vector<uint8_t> testPitches = {0, 64, 127, 128, 255}; // Valid and out-of-bounds
            std::vector<uint8_t> testVelocities = {0, 32, 127, 128, 255}; // Valid and out-of-bounds

            // Iterate through the combinations of test cases
            for (uint8_t channel : testChannels) {
                for (uint8_t pitch : testPitches) {
                    for (uint8_t velocity : testVelocities) {
                        // Create and validate the Note On message
                        msg.NoteOn(pitch, velocity, channel);
                        ValidateNoteOnMessage(msg, channel, pitch, velocity);
                    }
                }
            }
        }
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Note Off

            // Utility function to validate a Note Off message
            void ValidateNoteOffMessage(Message& msg, uint8_t channel, uint8_t pitch, uint8_t velocity) {
                // Clamp values to their valid MIDI ranges
                uint8_t expectedChannel = Clamp(channel, 0, 15);   // MIDI channels are 0–15
                uint8_t expectedPitch = Clamp(pitch, 0, 127);      // MIDI pitches are 0–127
                uint8_t expectedVelocity = Clamp(velocity, 0, 127); // MIDI velocities are 0–127

                // Verify the message structure
                EXPECT_EQ(msg.size(), 3); // Note Off messages are always 3 bytes
                EXPECT_EQ(msg.Data(0), MIDI_NOTE_OFF + expectedChannel); // Command + Channel
                EXPECT_EQ(msg.Data(1), expectedPitch);                  // Pitch
                EXPECT_EQ(msg.Data(2), expectedVelocity);               // Velocity
            }

            // NoteOffMessage Test
            TEST(MidiMessageTest, NoteOffMessage) {
                Message msg;

                // Define a set of test cases for channel, pitch, and velocity
                std::vector<uint8_t> testChannels = {0, 7, 15, 16, 255}; // Min, mid, max, and out-of-bounds
                std::vector<uint8_t> testPitches = {0, 64, 127, 128, 255}; // Min, mid, max, and out-of-bounds
                std::vector<uint8_t> testVelocities = {0, 32, 127, 128, 255}; // Min, mid, max, and out-of-bounds

                // Iterate through all combinations of test cases
                for (uint8_t channel : testChannels) {
                    for (uint8_t pitch : testPitches) {
                        for (uint8_t velocity : testVelocities) {
                            // Construct the Note Off message
                            msg.NoteOff(pitch, velocity, channel);

                            // Validate the Note Off message
                            ValidateNoteOffMessage(msg, channel, pitch, velocity);
                        }
                    }
                }
            }
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // After Touch

            // Utility function to validate an AfterTouch message
            void ValidateAfterTouchMessage(Message& msg, uint8_t channel, uint8_t pitch, uint8_t pressure) {
                // Clamp values to their valid MIDI ranges
                uint8_t expectedChannel = Clamp(channel, 0, 15);   // MIDI channels are 0–15
                uint8_t expectedPitch = Clamp(pitch, 0, 127);      // MIDI pitches are 0–127
                uint8_t expectedPressure = Clamp(pressure, 0, 127); // MIDI pressures are 0–127

                // Verify the message structure
                EXPECT_EQ(msg.size(), 3); // AfterTouch messages are always 3 bytes
                EXPECT_EQ(msg.Data(0), MIDI_AFTER_TOUCH + expectedChannel); // Command + Channel
                EXPECT_EQ(msg.Data(1), expectedPitch);                      // Pitch
                EXPECT_EQ(msg.Data(2), expectedPressure);                   // Pressure
            }

            //  AfterTouchMessage Test
            TEST(MidiMessageTest, AfterTouchMessage) {
                Message msg;

                // Define a set of test cases for channel, pitch, and pressure
                std::vector<uint8_t> testChannels = {0, 7, 15, 16, 255}; // Min, mid, max, and out-of-bounds
                std::vector<uint8_t> testPitches = {0, 64, 127, 128, 255}; // Min, mid, max, and out-of-bounds
                std::vector<uint8_t> testPressures = {0, 32, 127, 128, 255}; // Min, mid, max, and out-of-bounds

                // Iterate through all combinations of test cases
                for (uint8_t channel : testChannels) {
                    for (uint8_t pitch : testPitches) {
                        for (uint8_t pressure : testPressures) {
                            // Construct the AfterTouch message
                            msg.AfterTouch(pitch, pressure, channel);

                            // Validate the AfterTouch message
                            ValidateAfterTouchMessage(msg, channel, pitch, pressure);
                        }
                    }
                }
            }
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Control Change

            // Utility function to validate a Control Change message
            void ValidateControlChangeMessage(Message& msg, uint8_t channel, uint8_t controllerNumber, uint8_t value) {
                // Clamp values to their valid MIDI ranges
                uint8_t expectedChannel = Clamp(channel, 0, 15);                 // MIDI channels are 0–15
                uint8_t expectedControllerNumber = Clamp(controllerNumber, 0, 127); // MIDI controller numbers are 0–127
                uint8_t expectedValue = Clamp(value, 0, 127);                     // MIDI values are 0–127

                // Verify the message structure
                EXPECT_EQ(msg.size(), 3); // Control Change messages are always 3 bytes
                EXPECT_EQ(msg.Data(0), MIDI_CONTROL_CHANGE + expectedChannel); // Command + Channel
                EXPECT_EQ(msg.Data(1), expectedControllerNumber);              // Controller Number
                EXPECT_EQ(msg.Data(2), expectedValue);                         // Value
            }

            // Refactored ControlChangeMessage Test
            TEST(MidiMessageTest, ControlChangeMessage) {
            Message msg;

            // Define a set of test cases for channel, controller number, and value
            std::vector<uint8_t> testChannels = {0, 7, 15, 16, 255};         // Min, mid, max, and out-of-bounds
            std::vector<uint8_t> testControllerNumbers = {0, 64, 127, 128, 255}; // Min, mid, max, and out-of-bounds
            std::vector<uint8_t> testValues = {0, 32, 127, 128, 255};         // Min, mid, max, and out-of-bounds

            // Iterate through all combinations of test cases
            for (uint8_t channel : testChannels) {
                for (uint8_t controllerNumber : testControllerNumbers) {
                    for (uint8_t value : testValues) {
                        // Construct the Control Change message
                        msg.ControlChange(controllerNumber, value, channel);

                        // Validate the Control Change message
                        ValidateControlChangeMessage(msg, channel, controllerNumber, value);
                    }
                }
            }
        }
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Program Change messages

            // Utility function to validate a Program Change message
            void ValidateProgramChangeMessage(Message& msg, uint8_t channel, uint8_t program) {
                // Clamp values to their valid MIDI ranges
                uint8_t expectedChannel = Clamp(channel, 0, 15);   // MIDI channels are 0–15
                uint8_t expectedProgram = Clamp(program, 0, 127);  // MIDI programs are 0–127

                // Verify the message structure
                EXPECT_EQ(msg.size(), 2); // Program Change messages are always 2 bytes
                EXPECT_EQ(msg.Data(0), MIDI_PROGRAM_CHANGE + expectedChannel); // Command + Channel
                EXPECT_EQ(msg.Data(1), expectedProgram);                       // Program Number
            }

            // Refactored ProgramChangeMessage Test
            TEST(MidiMessageTest, ProgramChangeMessage) {
                Message msg;

                // Define a set of test cases for channel and program
                std::vector<uint8_t> testChannels = {0, 7, 15, 16, 255}; // Min, mid, max, and out-of-bounds
                std::vector<uint8_t> testPrograms = {0, 64, 127, 128, 255}; // Min, mid, max, and out-of-bounds

                // Iterate through all combinations of test cases
                for (uint8_t channel : testChannels) {
                    for (uint8_t program : testPrograms) {
                        // Construct the Program Change message
                        msg.ProgramChange(program, channel);

                        // Validate the Program Change message
                        ValidateProgramChangeMessage(msg, channel, program);
                    }
                }
            }
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Channel Pressure

            // Utility function to validate a Channel Pressure message
            void ValidateChannelPressureMessage(Message& msg, uint8_t channel, uint8_t pressure) {
                // Clamp values to their valid MIDI ranges
                uint8_t expectedChannel = Clamp(channel, 0, 15);   // MIDI channels are 0–15
                uint8_t expectedPressure = Clamp(pressure, 0, 127); // MIDI pressure values are 0–127

                // Verify the message structure
                EXPECT_EQ(msg.size(), 2); // Channel Pressure messages are always 2 bytes
                EXPECT_EQ(msg.Data(0), MIDI_CHANNEL_PRESSURE + expectedChannel); // Command + Channel
                EXPECT_EQ(msg.Data(1), expectedPressure);                        // Pressure Value
            }

            // Refactored ChannelPressureMessage Test
            TEST(MidiMessageTest, ChannelPressureMessage) {
            Message msg;

            // Define a set of test cases for channel and pressure
            std::vector<uint8_t> testChannels = {0, 7, 15, 16, 255}; // Min, mid, max, and out-of-bounds
            std::vector<uint8_t> testPressures = {0, 64, 127, 128, 255}; // Min, mid, max, and out-of-bounds

            // Iterate through all combinations of test cases
            for (uint8_t channel : testChannels) {
                for (uint8_t pressure : testPressures) {
                    // Construct the Channel Pressure message
                    msg.ChannelPressure(pressure, channel);

                    // Validate the Channel Pressure message
                    ValidateChannelPressureMessage(msg, channel, pressure);
                }
            }
        }
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Pitch Bend

            TEST(MidiMessageTest, PitchBendMessageSigned16Bit) {
                Message msg;
                int16_t value = INT16_MIN;
                uint8_t channel = 0;
                const uint16_t midpoint = 8192;  // 14-bit midpoint for no pitch bend

                do {
                    channel = 0;
                    do {
                        msg.PitchBend(value, channel);

                        // Clamp values to [-8192, 8191] for pitch bend
                        int16_t adjustedValue = Clamp(value, -8192, 8191); // Keep 14 bits, including the sign
                        
                        // Apply sign to adjust around the midpoint
                        uint16_t val = static_cast<uint16_t>(adjustedValue + midpoint);

                        // Verify the size and command bytes
                        EXPECT_EQ(msg.size(), 3);
                        EXPECT_EQ(msg.Data(0), MIDI_PITCH_BEND + Clamp(channel, 0, 15)); // Pitch Bend + channel

                        // Verify the LSB and MSB for the clamped value
                        EXPECT_EQ(msg.Data(1), static_cast<uint8_t>(val & 0x7F)); // LSB
                        EXPECT_EQ(msg.Data(2), static_cast<uint8_t>((val >> 7) & 0x7F)); // MSB

                        ++channel;
                    } while (channel != 0); // Loop through all uint8_t values, including 255.
                    ++value;
                } while (value != INT16_MIN); // Stop when we wrap around
            }
            
            TEST(MidiMessageTest, PitchBendMessageSigned8Bit) {
                Message msg;
                int8_t value = INT8_MIN;
                const uint16_t midpoint = 8192;  // 14-bit midpoint for no pitch bend

                do {
                    uint8_t channel = 0;
                    do {
                        msg.PitchBend(value, channel);

                        uint16_t adjustedValue = (static_cast<uint16_t>(abs(value)) << 6);  // Shift by 6 bits for 14-bit scaling

                        // Apply sign to adjust around the midpoint
                        uint16_t val = (value >= 0) ? (midpoint + adjustedValue) : (midpoint - adjustedValue);

                        // Verify the size and command bytes
                        EXPECT_EQ(msg.size(), 3);
                        EXPECT_EQ(msg.Data(0), MIDI_PITCH_BEND + Clamp(channel, 0, 15)); // Pitch Bend + channel

                        // Verify the LSB and MSB for the clamped value
                        EXPECT_EQ(msg.Data(1), static_cast<uint8_t>(val & 0x7F)); // LSB
                        EXPECT_EQ(msg.Data(2), static_cast<uint8_t>((val >> 7) & 0x7F)); // MSB

                        ++channel;
                    } while (channel != 0); // Loop through all uint8_t values, including 255.
                    ++value;
                } while (value != INT8_MIN); // Stop when we wrap around
            }

            TEST(MidiMessageTest, PitchBendMessageUnsigned16Bit) {
                Message     msg;
                uint16_t value = 0;

                do {
                    uint8_t channel = 0;
                    do {
                        msg.PitchBend(value, 1);

                        // Clamp the expected channel
                        uint8_t expectedChannel = Clamp(channel, 0, 15);

                        // Verify the size and command bytes
                        EXPECT_EQ(msg.size(), 3);
                        EXPECT_EQ(msg.Data(0), 0xE0 | (1 & 0x0F)); // Pitch Bend command for Channel 1 (0xE0)

                        // Clamp the value to 14-bit range (0 to 0x3FFF)
                        uint16_t clampedValue = (value > 0x3FFF) ? 0x3FFF : value;

                        // Verify the LSB and MSB for the clamped value
                        EXPECT_EQ(msg.Data(1), static_cast<uint8_t>(clampedValue & 0x7F)); // LSB
                        EXPECT_EQ(msg.Data(2), static_cast<uint8_t>((clampedValue >> 7) & 0x7F)); // MSB

                        ++channel;
                    } while (channel != 0); // Loop through all uint8_t values, including 255.
                    ++value;
                } while (value != 0); // Stop when we wrap around
            }

            TEST(MidiMessageTest, PitchBendMessageUnsigned8Bit) {
                Message msg;
                uint8_t value = 0;

                do {
                    uint8_t channel = 0;
                    do {
                        msg.PitchBend(value, channel);

                        // Clamp the expected channel
                        uint8_t expectedChannel = Clamp(channel, 0, 15);
                        
                        // Map the 8-bit value to the 14-bit range
                        uint16_t mappedValue = static_cast<uint16_t>(value) << 6;

                        // Verify the size and command bytes
                        EXPECT_EQ(msg.size(), 3);
                        EXPECT_EQ(msg.Data(0), MIDI_PITCH_BEND + expectedChannel); // Pitch Bend command for Channel 1 (0xE0)

                        // Verify the LSB and MSB for the mapped value
                        EXPECT_EQ(msg.Data(1), static_cast<uint8_t>(mappedValue & 0x7F)); // LSB
                        EXPECT_EQ(msg.Data(2), static_cast<uint8_t>((mappedValue >> 7) & 0x7F)); // MSB

                        ++channel;
                    } while (channel != 0); // Loop through all uint8_t values, including 255.
                    ++value;
                } while (value != 0); // Stop when we wrap around
            }

        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Channel Mode Messages

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // All Sound Off

            TEST(MidiMessageTest, SetAllSoundOff) {
                Message msg;

                // Iterate through all possible uint8_t channel values
                for (uint8_t channel = 0; channel < 255; ++channel) {
                    // Call the SetAllSoundOff API
                    msg.CC_AllSoundOff(channel);

                    // Clamp the channel to the valid MIDI range [0, 15]
                    uint8_t expectedChannel = (channel > 15) ? 15 : channel;

                    // Verify the message structure
                    EXPECT_EQ(msg.size(), 3); // All Sound Off messages are always 3 bytes
                    EXPECT_EQ(msg.Data(0), MIDI_CONTROL_CHANGE + expectedChannel); // Control Change + Clamped Channel
                    EXPECT_EQ(msg.Data(1), MIDI_ALL_SOUND_OFF);                    // All Sound Off Control Number
                    EXPECT_EQ(msg.Data(2), 0);                                     // All Sound Off messages always have a value of 0
                }
            }
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // All Notes Off

            TEST(MidiMessageTest, SetAllNotesOff){
                Message msg;

                // Iterate through all possible uint8_t channel values
                for (uint8_t channel = 0; channel < 255; ++channel) {
                    // Call the SetAllNotesOff API
                    msg.CC_AllNotesOff(channel);

                    // Clamp the channel to the valid MIDI range [0, 15]
                    uint8_t expectedChannel = (channel > 15) ? 15 : channel;

                    // Verify the message structure
                    EXPECT_EQ(msg.size(), 3); // All Notes Off messages are always 3 bytes
                    EXPECT_EQ(msg.Data(0), MIDI_CONTROL_CHANGE + expectedChannel); // Control Change + Clamped Channel
                    EXPECT_EQ(msg.Data(1), MIDI_ALL_NOTES_OFF);                    // All Sound Off Control Number
                    EXPECT_EQ(msg.Data(2), 0);                                     // All Sound Off messages always have a value of 0
                }

            }
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Local Control

            TEST(MidiMessageTest, LocalControl) {
                Message msg;

                uint8_t channel = 0;
                do {
                    uint8_t expectedChannel = (channel > 15) ? 15 : channel; // Clamp channel to [0, 15]

                    // Helper lambda for validation
                    auto validateMessage = [&](uint8_t expectedMode) {
                        EXPECT_EQ(msg.size(), 3);
                        EXPECT_EQ(msg.Data(0), MIDI_CONTROL_CHANGE + expectedChannel); // ControlChange + Clamped Channel
                        EXPECT_EQ(msg.Data(1), MIDI_LOCAL_CONTROL);                    // Local Control
                        EXPECT_EQ(msg.Data(2), expectedMode);                          // Mode (On/Off)
                    };

                    // Test LocalControlStatus::On/Off
                    msg.CC_LocalControl(MidiProtocol::LocalControlStatus::On, channel);
                    validateMessage(MIDI_LOCAL_CONTROL_ON);

                    msg.CC_LocalControl(MidiProtocol::LocalControlStatus::Off, channel);
                    validateMessage(MIDI_LOCAL_CONTROL_OFF);

                    // Test uint8_t Mode
                    msg.CC_LocalControl(static_cast<uint8_t>(MIDI_LOCAL_CONTROL_ON), channel);
                    validateMessage(MIDI_LOCAL_CONTROL_ON);

                    msg.CC_LocalControl(static_cast<uint8_t>(MIDI_LOCAL_CONTROL_OFF), channel);
                    validateMessage(MIDI_LOCAL_CONTROL_OFF);

                    // Test bool Mode
                    msg.CC_LocalControl(true, channel);
                    validateMessage(MIDI_LOCAL_CONTROL_ON);

                    msg.CC_LocalControl(false, channel);
                    validateMessage(MIDI_LOCAL_CONTROL_OFF);

                    // Test direct CC_LocalControlOn/Off
                    msg.CC_LocalControlOn(channel);
                    validateMessage(MIDI_LOCAL_CONTROL_ON);

                    msg.CC_LocalControlOff(channel);
                    validateMessage(MIDI_LOCAL_CONTROL_OFF);

                    ++channel;
                } while (channel != 0); // Loop through all uint8_t values, including 255.
            }
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Omni

            TEST(MidiMessageTest, OmniMode) {
                Message msg;

                uint8_t channel = 0;
                do {
                    uint8_t expectedChannel = (channel > 15) ? 15 : channel; // Clamp channel to [0, 15]

                    // Helper lambda for validation
                    auto validateMessage = [&](uint8_t expectedMode) {
                        EXPECT_EQ(msg.size(), 3);
                        EXPECT_EQ(msg.Data(0), MIDI_CONTROL_CHANGE + expectedChannel); // ControlChange + Clamped Channel
                        EXPECT_EQ(msg.Data(1), expectedMode);                        // Omni Mode Control
                        EXPECT_EQ(msg.Data(2), 0);                          // Mode (On/Off)
                    };

                    // Test MidiProtocol::ChannelMode::OmniOn/OmniOff
                    msg.CC_OmniMode(MidiProtocol::ChannelMode::OmniOn, channel);
                    validateMessage(MIDI_OMNI_ON);

                    msg.CC_OmniMode(MidiProtocol::ChannelMode::OmniOff, channel);
                    validateMessage(MIDI_OMNI_OFF);

                    // Test bool Mode
                    msg.CC_OmniMode(true, channel);
                    validateMessage(MIDI_OMNI_ON);

                    msg.CC_OmniMode(false, channel);
                    validateMessage(MIDI_OMNI_OFF);
                    
                    // Test uint8_t Mode
                    msg.CC_OmniMode(static_cast<uint8_t>(MIDI_OMNI_ON), channel);
                    validateMessage(MIDI_OMNI_ON);

                    msg.CC_OmniMode(static_cast<uint8_t>(MIDI_OMNI_OFF), channel);
                    validateMessage(MIDI_OMNI_OFF);

                    // Test direct CC_OmniOn/CC_OmniOff
                    msg.CC_OmniOn(channel);
                    validateMessage(MIDI_OMNI_ON);

                    msg.CC_OmniOff(channel);
                    validateMessage(MIDI_OMNI_OFF);

                    ++channel;
                } while (channel != 0); // Loop through all uint8_t values, including 255.
            }
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Polyphony

            TEST(MidiMessageTest, PolyphonyMode) {
                Message msg;

                uint8_t channel = 0;
                do {
                    uint8_t expectedChannel = (channel > 15) ? 15 : channel; // Clamp channel to [0, 15]

                    // Helper lambda for validation
                    auto validateMessage = [&](uint8_t expectedMode) {
                        EXPECT_EQ(msg.size(), 3);
                        EXPECT_EQ(msg.Data(0), MIDI_CONTROL_CHANGE + expectedChannel); // ControlChange + Clamped Channel
                        EXPECT_EQ(msg.Data(1), expectedMode);                   // Polyphony Mode Control
                        EXPECT_EQ(msg.Data(2), 0);                          // Mode (Mono/Poly)
                    };

                    // Test MidiProtocol::ChannelMode::Mono/Poly
                    msg.CC_Polyphony(MidiProtocol::ChannelMode::Mono, channel);
                    validateMessage(MIDI_MONO_ON);

                    msg.CC_Polyphony(MidiProtocol::ChannelMode::Poly, channel);
                    validateMessage(MIDI_POLY_ON);

                    // Test bool Mode
                    msg.CC_Polyphony(false, channel);
                    validateMessage(MIDI_MONO_ON);

                    msg.CC_Polyphony(true, channel);
                    validateMessage(MIDI_POLY_ON);

                    // Test uint8_t Mode
                    msg.CC_Polyphony(static_cast<uint8_t>(MIDI_MONO_ON), channel);
                    validateMessage(MIDI_MONO_ON);

                    msg.CC_Polyphony(static_cast<uint8_t>(MIDI_POLY_ON), channel);
                    validateMessage(MIDI_POLY_ON);

                    // Test direct SetMono/SetPoly
                    msg.CC_Mono(channel);
                    validateMessage(MIDI_MONO_ON);

                    msg.CC_Poly(channel);
                    validateMessage(MIDI_POLY_ON);

                    ++channel;
                } while (channel != 0); // Loop through all uint8_t values, including 255.
            }
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // System Common Messages


    //
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    }  // namespace
}  // namespace MIDILAR::MidiFoundation