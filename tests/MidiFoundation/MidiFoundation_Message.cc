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

            original = std::move(moved);
            EXPECT_EQ(original.size(), 3);
            EXPECT_EQ(original.Data(0), 0x91);
            EXPECT_EQ(original.Data(1), 60);
            EXPECT_EQ(original.Data(2), 100);
        }

        TEST(MidiMessageTest, RawBufferConstructor) {
            uint8_t buffer[] = {0x90, 60, 100};
            Message msg(buffer, sizeof(buffer));

            EXPECT_EQ(msg.size(), 3);
            EXPECT_EQ(msg.Data(0), 0x90);
            EXPECT_EQ(msg.Data(1), 60);
            EXPECT_EQ(msg.Data(2), 100);
        }
        
        TEST(MidiMessageTest, AssignOperator) {
            uint8_t buffer[] = {0x90, 60, 100};
            Message msg(buffer, sizeof(buffer));

            Message msg2;
            msg2 = msg;

            EXPECT_EQ(msg2.size(), 3);
            EXPECT_EQ(msg2.Data(0), 0x90);
            EXPECT_EQ(msg2.Data(1), 60);
            EXPECT_EQ(msg2.Data(2), 100);
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
        
        TEST(MidiMessageTest, VectorAssignment){
            std::vector<uint8_t> data = {0x90, 60, 100};
            Message msg;
            msg = data;

            EXPECT_EQ(msg.size(), 3);
            EXPECT_EQ(msg.Data(0), 0x90);
            EXPECT_EQ(msg.Data(1), 60);
            EXPECT_EQ(msg.Data(2), 100);
        }
        
        TEST(MidiMessageTest, VectorSetRawData){
            std::vector<uint8_t> data = {0x90, 60, 100};
            Message msg;
            msg.SetRawData(data);

            EXPECT_EQ(msg.size(), 3);
            EXPECT_EQ(msg.Data(0), 0x90);
            EXPECT_EQ(msg.Data(1), 60);
            EXPECT_EQ(msg.Data(2), 100);

            data.clear();

            msg.SetRawData(data);
            EXPECT_EQ(msg.size(), 0);

        }
        
        TEST(MidiMessageTest, VectorMoveAssignment){
            std::vector<uint8_t> data = {0x90, 60, 100};
            Message msg;
            msg = std::move(data);

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
            EXPECT_EQ(msg.Data(3), 0); // Out of Bounds check
        }

        TEST(MidiMessageTest, SizeAccess) {
            Message msg;
            EXPECT_EQ(msg.size(), 0);

            msg.NoteOn(60, 100, 1);
            EXPECT_EQ(msg.size(), 3);
        }

        TEST(MidiMessageTest, SetRawData){
            uint8_t rawData [3] = {
                0x90,
                120,
                120
            };

            Message msg;

            msg.SetRawData(rawData,3);

            // Validate message contents
            EXPECT_EQ(msg.size(), 3);
            EXPECT_EQ(msg.Data(0), rawData[0]); // Status Byte
            EXPECT_EQ(msg.Data(1), rawData[1]); // Data Byte 1
            EXPECT_EQ(msg.Data(2), rawData[2]); // Data Byte 2

            msg.SetRawData(nullptr,0);
            EXPECT_EQ(msg.size(), 0);
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
            auto it = message.cbegin();
            auto end = message.cend();

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
        
            TEST(MidiMessageTest, CC_Specializations) {
                Message msg;
            
                uint8_t Values[3] = {0x00, 0x7F, 0x80}; // Test boundary values
                uint8_t Channels[3] = {0x00, 0x0F, 0x10}; // Valid & out-of-range channels
            
                for (size_t i = 0; i < 3; i++) {
                    for (size_t j = 0; j < 3; j++) {
                        
                        uint8_t expectedChannel = (Channels[i] < 15) ? Channels[i]:15;
                        uint8_t expectedValue = (Values[j] < 127) ? Values[j]:127;

                        // Test CC_BankSelect
                        msg = msg.CC_BankSelect(Values[j], Channels[i]);
                        EXPECT_EQ(msg.Data(0), 0xB0 + expectedChannel);
                        EXPECT_EQ(msg.Data(1), MIDI_BANK_SELECT);
                        EXPECT_EQ(msg.Data(2), expectedValue);
            
                        // Test CC_Modulation
                        msg = msg.CC_Modulation(Values[j], Channels[i]);
                        EXPECT_EQ(msg.Data(0), 0xB0 + expectedChannel);
                        EXPECT_EQ(msg.Data(1), MIDI_MODULATION);
                        EXPECT_EQ(msg.Data(2), expectedValue);
            
                        // Test CC_BreathControl
                        msg = msg.CC_BreathControl(Values[j], Channels[i]);
                        EXPECT_EQ(msg.Data(0), 0xB0 + expectedChannel);
                        EXPECT_EQ(msg.Data(1), MIDI_BREATH_CONTROL);
                        EXPECT_EQ(msg.Data(2), expectedValue);
            
                        // Test CC_FootPedal
                        msg = msg.CC_FootPedal(Values[j], Channels[i]);
                        EXPECT_EQ(msg.Data(0), 0xB0 + expectedChannel);
                        EXPECT_EQ(msg.Data(1), MIDI_FOOT_PEDAL);
                        EXPECT_EQ(msg.Data(2), expectedValue);
            
                        // Test CC_Portamento
                        msg = msg.CC_Portamento(Values[j], Channels[i]);
                        EXPECT_EQ(msg.Data(0), 0xB0 + expectedChannel);
                        EXPECT_EQ(msg.Data(1), MIDI_PORTAMENTO);
                        EXPECT_EQ(msg.Data(2), expectedValue);
            
                        // Test CC_Volume
                        msg = msg.CC_Volume(Values[j], Channels[i]);
                        EXPECT_EQ(msg.Data(0), 0xB0 + expectedChannel);
                        EXPECT_EQ(msg.Data(1), MIDI_VOLUME);
                        EXPECT_EQ(msg.Data(2), expectedValue);
            
                        // Test CC_Balance
                        msg = msg.CC_Balance(Values[j], Channels[i]);
                        EXPECT_EQ(msg.Data(0), 0xB0 + expectedChannel);
                        EXPECT_EQ(msg.Data(1), MIDI_BALANCE);
                        EXPECT_EQ(msg.Data(2), expectedValue);
            
                        // Test CC_Pan
                        msg = msg.CC_Pan(Values[j], Channels[i]);
                        EXPECT_EQ(msg.Data(0), 0xB0 + expectedChannel);
                        EXPECT_EQ(msg.Data(1), MIDI_PAN);
                        EXPECT_EQ(msg.Data(2), expectedValue);
            
                        // Test CC_Expression
                        msg = msg.CC_Expression(Values[j], Channels[i]);
                        EXPECT_EQ(msg.Data(0), 0xB0 + expectedChannel);
                        EXPECT_EQ(msg.Data(1), MIDI_EXPRESSION);
                        EXPECT_EQ(msg.Data(2), expectedValue);
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

                    msg.CC_OmniMode(static_cast<uint8_t>(0x05), channel); // Invalid OmniMode value
                    EXPECT_EQ(msg.size(), 0); // _resize(0) should have cleared the message
            
                    msg.CC_OmniMode(static_cast<uint8_t>(0xFF), channel); // Completely out-of-range value
                    EXPECT_EQ(msg.size(), 0);
            
                    msg.CC_OmniMode(static_cast<MidiProtocol::ChannelMode>(99), channel); // Invalid enum case
                    EXPECT_EQ(msg.size(), 0);

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

                    msg.CC_Polyphony(static_cast<uint8_t>(0x05), channel); // Arbitrary invalid value
                    EXPECT_EQ(msg.size(), 0); // _resize(0) should have cleared the message
            
                    msg.CC_Polyphony(static_cast<uint8_t>(0xFF), channel); // Completely out-of-range value
                    EXPECT_EQ(msg.size(), 0);
            
                    msg.CC_Polyphony(static_cast<MidiProtocol::ChannelMode>(99), channel); // Invalid enum case
                    EXPECT_EQ(msg.size(), 0);

                    ++channel;
                } while (channel != 0); // Loop through all uint8_t values, including 255.
            }
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Real Time Messages

        TEST(MidiMessageTest, RealTimeMessages) {
            Message msg;
        
            // Test TimingTick
            msg = msg.TimingTick();
            EXPECT_EQ(msg.size(), 1);
            EXPECT_EQ(msg.Data(0), MIDI_REALTIME_TIMING_TICK);
        
            // Test Start
            msg = msg.Start();
            EXPECT_EQ(msg.size(), 1);
            EXPECT_EQ(msg.Data(0), MIDI_REALTIME_START);
        
            // Test Continue
            msg = msg.Continue();
            EXPECT_EQ(msg.size(), 1);
            EXPECT_EQ(msg.Data(0), MIDI_REALTIME_CONTINUE);
        
            // Test Stop
            msg = msg.Stop();
            EXPECT_EQ(msg.size(), 1);
            EXPECT_EQ(msg.Data(0), MIDI_REALTIME_STOP);
        
            // Test ActiveSensing
            msg = msg.ActiveSensing();
            EXPECT_EQ(msg.size(), 1);
            EXPECT_EQ(msg.Data(0), MIDI_REALTIME_ACTIVE_SENSING);
        
            // Test SystemReset
            msg = msg.SystemReset();
            EXPECT_EQ(msg.size(), 1);
            EXPECT_EQ(msg.Data(0), MIDI_REALTIME_SYSTEM_RESET);
        }
    
    //
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // System Common Messages

        TEST(MidiMessageTest, SongPositionPointer) {
            Message msg;
        
            // Test different valid positions (14-bit values)
            uint16_t testPositions[] = {0x0000, 0x007F, 0x3FFF}; // Min, boundary, max
        
            for (uint16_t position : testPositions) {
                msg = msg.SongPositionPointer(position);
        
                // Ensure the message is correctly resized
                ASSERT_EQ(msg.size(), 3) << "Incorrect message size for SongPositionPointer";
        
                // Validate message contents
                EXPECT_EQ(msg.Data(0), MIDI_SONG_POSITION_POINTER); // Status byte
                EXPECT_EQ(msg.Data(1), position & 0x7F);            // LSB (Least Significant Byte)
                EXPECT_EQ(msg.Data(2), (position >> 7) & 0x7F);     // MSB (Most Significant Byte)
            }
        }
        
        TEST(MidiMessageTest, SongSelect) {
            Message msg;
        
            // Test valid song numbers (7-bit values)
            uint8_t testSongs[] = {0x00, 0x7F}; // Min and max valid values
        
            for (uint8_t song : testSongs) {
                msg = msg.SongSelect(song);
        
                // Ensure the message is correctly resized
                ASSERT_EQ(msg.size(), 2) << "Incorrect message size for SongSelect";
        
                // Validate message contents
                EXPECT_EQ(msg.Data(0), MIDI_SONG_SELECT); // Status byte
                EXPECT_EQ(msg.Data(1), song & 0x7F);      // Valid 7-bit song number
            }
        }
        
        TEST(MidiMessageTest, TuningRequest) {
            Message msg;
        
            msg = msg.TuningRequest();
        
            // Ensure the message is correctly resized
            ASSERT_EQ(msg.size(), 1) << "Incorrect message size for TuningRequest";
        
            // Validate message contents
            EXPECT_EQ(msg.Data(0), MIDI_TUNING_REQUEST); // Status byte
        }
        
        TEST(MidiMessageTest, SystemExclusive) {
            Message msg;
        
            // Test Case 1: Valid SysEx message (Already Includes Start and End Bytes)
            uint8_t validSysEx[] = {0xF0, 0x01, 0x02, 0x03, 0xF7};
            msg = msg.SystemExclusive(validSysEx, sizeof(validSysEx));
        
            ASSERT_EQ(msg.size(), 5) << "Incorrect message size for valid SysEx";
            EXPECT_EQ(msg.Data(0), 0xF0);
            EXPECT_EQ(msg.Data(1), 0x01);
            EXPECT_EQ(msg.Data(2), 0x02);
            EXPECT_EQ(msg.Data(3), 0x03);
            EXPECT_EQ(msg.Data(4), 0xF7);
        
            // Test Case 2: SysEx Missing Start Byte (Should Add 0xF0)
            uint8_t missingStartSysEx[] = {0x01, 0x02, 0x03, 0xF7};
            msg = msg.SystemExclusive(missingStartSysEx, sizeof(missingStartSysEx));
        
            ASSERT_EQ(msg.size(), 5) << "Incorrect message size when missing 0xF0";
            EXPECT_EQ(msg.Data(0), 0xF0);
            EXPECT_EQ(msg.Data(1), 0x01);
            EXPECT_EQ(msg.Data(2), 0x02);
            EXPECT_EQ(msg.Data(3), 0x03);
            EXPECT_EQ(msg.Data(4), 0xF7);
        
            // Test Case 3: SysEx Missing End Byte (Should Add 0xF7)
            uint8_t missingEndSysEx[] = {0xF0, 0x01, 0x02, 0x03};
            msg = msg.SystemExclusive(missingEndSysEx, sizeof(missingEndSysEx));
        
            ASSERT_EQ(msg.size(), 5) << "Incorrect message size when missing 0xF7";
            EXPECT_EQ(msg.Data(0), 0xF0);
            EXPECT_EQ(msg.Data(1), 0x01);
            EXPECT_EQ(msg.Data(2), 0x02);
            EXPECT_EQ(msg.Data(3), 0x03);
            EXPECT_EQ(msg.Data(4), 0xF7);
        
            // Test Case 4: SysEx Missing Both Start and End Bytes (Should Add Both)
            uint8_t missingBothSysEx[] = {0x01, 0x02, 0x03};
            msg = msg.SystemExclusive(missingBothSysEx, sizeof(missingBothSysEx));
        
            ASSERT_EQ(msg.size(), 5) << "Incorrect message size when missing both 0xF0 and 0xF7";
            EXPECT_EQ(msg.Data(0), 0xF0);
            EXPECT_EQ(msg.Data(1), 0x01);
            EXPECT_EQ(msg.Data(2), 0x02);
            EXPECT_EQ(msg.Data(3), 0x03);
            EXPECT_EQ(msg.Data(4), 0xF7);
        
            // Test Case 5: Empty Data (Should Not Modify Message)
            uint8_t emptyData[] = {};
            msg = msg.SystemExclusive(emptyData, 0);
        
            EXPECT_EQ(msg.size(), 5) << "SystemExclusive(0) should not modify the message"; // Expect previous state to persist
        }
        

    //
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // MTC

        TEST(MidiMessageTest, MTC_QuarterFrame) {
            Message msg;
        
            // Test different TimeComponents and Data values
            for (uint8_t timeComponent = 0; timeComponent < 8; timeComponent++) {
                for (uint8_t data = 0; data < 16; data++) {
                    msg = msg.MTC_QuarterFrame(timeComponent, data);
        
                    EXPECT_EQ(msg.size(), 2);
                    EXPECT_EQ(msg.Data(0), MIDI_MTC_QUARTER_FRAME);
                    EXPECT_EQ(msg.Data(1), ((timeComponent & 0x07) << 4) | (data & 0x0F));
                }
            }
        }
        
        TEST(MidiMessageTest, MTC_QuarterFrame_Enum) {
            Message msg;
        
            for (uint8_t data = 0; data < 16; data++) {
                msg = msg.MTC_QuarterFrame(MidiProtocol_MTC::TimeComponent::FramesLSB, data);
                EXPECT_EQ(msg.size(), 2);
                EXPECT_EQ(msg.Data(0), MIDI_MTC_QUARTER_FRAME);
                EXPECT_EQ(msg.Data(1), ((static_cast<uint8_t>(MidiProtocol_MTC::TimeComponent::FramesLSB) & 0x07) << 4) | (data & 0x0F));
            }
        }
        
        TEST(MidiMessageTest, MTC_FullFrame) {
            Message msg;
        
            uint8_t hours = 23;
            uint8_t minutes = 59;
            uint8_t seconds = 58;
            uint8_t frames = 29;
            uint8_t framerate = 1; // Assume SMPTE 25 fps
            uint8_t sysexChannel = 0x01;
        
            msg = msg.MTC_FullFrame(hours, minutes, seconds, frames, framerate, sysexChannel);
        
            EXPECT_EQ(msg.size(), 8);
            EXPECT_EQ(msg.Data(0), MIDI_SYSEX_START);     // Start of SysEx
            EXPECT_EQ(msg.Data(1), sysexChannel);         // SysEx Channel
            EXPECT_EQ(msg.Data(2), MIDI_SYSEX_RT_MTC_FULL_FRAME);
            EXPECT_EQ(msg.Data(3), ((framerate & 0x03) << 5) | (hours & 0x1F)); // Framerate + Hours
            EXPECT_EQ(msg.Data(4), minutes & 0x3F);       // Minutes
            EXPECT_EQ(msg.Data(5), seconds & 0x3F);       // Seconds
            EXPECT_EQ(msg.Data(6), frames & 0x1F);        // Frames
            EXPECT_EQ(msg.Data(7), MIDI_SYSEX_END);       // End of SysEx
        }
        
        TEST(MidiMessageTest, MTC_FullFrame_Enum) {
            Message msg;
        
            uint8_t hours = 12;
            uint8_t minutes = 34;
            uint8_t seconds = 56;
            uint8_t frames = 20;
            MidiProtocol_MTC::FrameRate framerate = MidiProtocol_MTC::FrameRate::FPS30;
            uint8_t sysexChannel = 0x02;
        
            msg = msg.MTC_FullFrame(hours, minutes, seconds, frames, framerate, sysexChannel);
        
            EXPECT_EQ(msg.size(), 8);
            EXPECT_EQ(msg.Data(0), MIDI_SYSEX_START);
            EXPECT_EQ(msg.Data(1), sysexChannel);
            EXPECT_EQ(msg.Data(2), MIDI_SYSEX_RT_MTC_FULL_FRAME);
            EXPECT_EQ(msg.Data(3), ((static_cast<uint8_t>(framerate) & 0x03) << 5) | (hours & 0x1F)); // Framerate + Hours
            EXPECT_EQ(msg.Data(4), minutes & 0x3F);
            EXPECT_EQ(msg.Data(5), seconds & 0x3F);
            EXPECT_EQ(msg.Data(6), frames & 0x1F);
            EXPECT_EQ(msg.Data(7), MIDI_SYSEX_END);
        }
    //
    }  // namespace
}  // namespace MIDILAR::MidiFoundation