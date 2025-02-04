#include <gtest/gtest.h>
#include <MidiFoundation/Protocol/Defines.h>
#include <MidiFoundation/Protocol/Enums.h>

namespace MIDILAR::MidiFoundation{
 
    namespace{
        class MidiFoundationTesting : public testing::Test{
            private:

            protected:
            // You can do set-up work for each test here.
            MidiFoundationTesting() {
            }

            // You can do clean-up work that doesn't throw exceptions here.
            ~MidiFoundationTesting() override {
                
            }

            // If the constructor and destructor are not enough for setting up
            // and cleaning up each test, you can define the following methods:
            void SetUp() override {
            }

            void TearDown() override {
            }
        };
    }

    // Unit test to verify that MIDI defines are correctly parsed by the compiler
    TEST(MidiFoundationTesting, Defines) {
        // Test that each defined constant has the expected value

        // Channel Voice Messages
        EXPECT_EQ(MIDI_NOTE_OFF, 0x80);
        EXPECT_EQ(MIDI_NOTE_ON, 0x90);
        EXPECT_EQ(MIDI_AFTER_TOUCH, 0xA0);
        EXPECT_EQ(MIDI_CONTROL_CHANGE, 0xB0);
        EXPECT_EQ(MIDI_PROGRAM_CHANGE, 0xC0);
        EXPECT_EQ(MIDI_CHANNEL_PRESSURE, 0xD0);
        EXPECT_EQ(MIDI_PITCH_BEND, 0xE0);
        
        // Channel Mode Messages
        EXPECT_EQ(MIDI_BANK_SELECT, 0x00);
        EXPECT_EQ(MIDI_MODULATION, 0x01);
        EXPECT_EQ(MIDI_BREATH_CONTROL, 0x02);
        EXPECT_EQ(MIDI_FOOT_PEDAL, 0x04);
        EXPECT_EQ(MIDI_PORTAMENTO, 0x05);
        EXPECT_EQ(MIDI_DATA_ENTRY_MSB, 0x06);
        EXPECT_EQ(MIDI_VOLUME, 0x07);
        EXPECT_EQ(MIDI_BALANCE, 0x08);
        EXPECT_EQ(MIDI_PAN, 0x0A);
        EXPECT_EQ(MIDI_EXPRESSION, 0x0B);
        EXPECT_EQ(MIDI_DATA_ENTRY_LSB, 0x26);
        
        // NRPN and RPN Messages
        EXPECT_EQ(MIDI_NRPN_DATA_INCREMENT, 0x60);
        EXPECT_EQ(MIDI_NRPN_DATA_DECREMENT, 0x61);
        EXPECT_EQ(MIDI_NRPN_LSB, 0x62);
        EXPECT_EQ(MIDI_NRPN_MSB, 0x63);
        EXPECT_EQ(MIDI_RPN_LSB, 0x64);
        EXPECT_EQ(MIDI_RPN_MSB, 0x65);

        // All Sound Off and Miscellaneous Commands
        EXPECT_EQ(MIDI_ALL_SOUND_OFF, 0x78);
        EXPECT_EQ(MIDI_RESET_ALL_CONTROLLERS, 0x79);
        EXPECT_EQ(MIDI_LOCAL_CONTROL, 0x7A);
        EXPECT_EQ(MIDI_ALL_NOTES_OFF, 0x7B);
        EXPECT_EQ(MIDI_OMNI_OFF, 0x7C);
        EXPECT_EQ(MIDI_OMNI_ON, 0x7D);
        EXPECT_EQ(MIDI_MONO_ON, 0x7E);
        EXPECT_EQ(MIDI_POLY_ON, 0x7F);
        EXPECT_EQ(MIDI_LOCAL_CONTROL_OFF, 0x00);
        EXPECT_EQ(MIDI_LOCAL_CONTROL_ON, 0x7F);

        // System Common Commands
        EXPECT_EQ(MIDI_SONG_POSITION_POINTER, 0xF2);
        EXPECT_EQ(MIDI_SONG_SELECT, 0xF3);
        EXPECT_EQ(MIDI_TUNING_REQUEST, 0xF6);

        // MTC Commands
        EXPECT_EQ(MIDI_MTC_QUARTER_FRAME, 0xF1);
        EXPECT_EQ(MIDI_MTC_FRAMES_LSB, 0x00);
        EXPECT_EQ(MIDI_MTC_FRAMES_MSB, 0x10);
        EXPECT_EQ(MIDI_MTC_SECONDS_LSB, 0x20);
        EXPECT_EQ(MIDI_MTC_SECONDS_MSB, 0x30);
        EXPECT_EQ(MIDI_MTC_MINUTES_LSB, 0x40);
        EXPECT_EQ(MIDI_MTC_MINUTES_MSB, 0x50);
        EXPECT_EQ(MIDI_MTC_HOURS_LSB, 0x60);
        EXPECT_EQ(MIDI_MTC_HOURS_MSB, 0x70);

        // MTC Frame Rates
        EXPECT_EQ(MIDI_MTC_FRAME_RATE_24FPS, 0b000);
        EXPECT_EQ(MIDI_MTC_FRAME_RATE_25FPS, 0b010);
        EXPECT_EQ(MIDI_MTC_FRAME_RATE_30FPS_DROP_FRAME, 0b100);
        EXPECT_EQ(MIDI_MTC_FRAME_RATE_30FPS, 0b110);

        // Real-Time MIDI Messages
        EXPECT_EQ(MIDI_REALTIME_TIMING_TICK, 0xF8);
        EXPECT_EQ(MIDI_REALTIME_START, 0xFA);
        EXPECT_EQ(MIDI_REALTIME_CONTINUE, 0xFB);
        EXPECT_EQ(MIDI_REALTIME_STOP, 0xFC);
        EXPECT_EQ(MIDI_REALTIME_ACTIVE_SENSING, 0xFE);
        EXPECT_EQ(MIDI_REALTIME_SYSTEM_RESET, 0xFF);

        // MIDI Exclusive Messages
        EXPECT_EQ(MIDI_SYSEX_START, 0xF0);
        EXPECT_EQ(MIDI_SYSEX_END, 0xF7);
    }

    TEST(MidiFoundationTesting, Enums){
        
        // Channel Voice Messages
        EXPECT_EQ(MIDI_NOTE_OFF,            (uint8_t) MidiProtocol::ChannelVoice::NoteOff);
        EXPECT_EQ(MIDI_NOTE_ON,             (uint8_t) MidiProtocol::ChannelVoice::NoteOn);
        EXPECT_EQ(MIDI_AFTER_TOUCH,         (uint8_t) MidiProtocol::ChannelVoice::AfterTouch);
        EXPECT_EQ(MIDI_CONTROL_CHANGE,      (uint8_t) MidiProtocol::ChannelVoice::ControlChange);
        EXPECT_EQ(MIDI_PROGRAM_CHANGE,      (uint8_t) MidiProtocol::ChannelVoice::ProgramChange);
        EXPECT_EQ(MIDI_CHANNEL_PRESSURE,    (uint8_t) MidiProtocol::ChannelVoice::ChannelPressure);
        EXPECT_EQ(MIDI_PITCH_BEND,          (uint8_t) MidiProtocol::ChannelVoice::PitchBend);
        
        // Channel Mode Messages
        EXPECT_EQ(MIDI_BANK_SELECT,     (uint8_t) MidiProtocol::ControlChange::BankSelect);
        EXPECT_EQ(MIDI_MODULATION,      (uint8_t) MidiProtocol::ControlChange::Modulation);
        EXPECT_EQ(MIDI_BREATH_CONTROL,  (uint8_t) MidiProtocol::ControlChange::BreathControl);
        EXPECT_EQ(MIDI_FOOT_PEDAL,      (uint8_t) MidiProtocol::ControlChange::FootPedal);
        EXPECT_EQ(MIDI_PORTAMENTO,      (uint8_t) MidiProtocol::ControlChange::Portamento);
        EXPECT_EQ(MIDI_DATA_ENTRY_MSB,  (uint8_t) MidiProtocol::ControlChange::DataEntryMsb);
        EXPECT_EQ(MIDI_VOLUME,          (uint8_t) MidiProtocol::ControlChange::Volume);
        EXPECT_EQ(MIDI_BALANCE,         (uint8_t) MidiProtocol::ControlChange::Balance);
        EXPECT_EQ(MIDI_PAN,             (uint8_t) MidiProtocol::ControlChange::Pan);
        EXPECT_EQ(MIDI_EXPRESSION,      (uint8_t) MidiProtocol::ControlChange::Expression);
        EXPECT_EQ(MIDI_DATA_ENTRY_LSB,  (uint8_t) MidiProtocol::ControlChange::DataEntryLsb);
        
        // NRPN and RPN Messages
        EXPECT_EQ(MIDI_NRPN_DATA_INCREMENT, (uint8_t) MidiProtocol::ControlChange::NrpnDataIncrement);
        EXPECT_EQ(MIDI_NRPN_DATA_DECREMENT, (uint8_t) MidiProtocol::ControlChange::NrpnDataDecrement);
        EXPECT_EQ(MIDI_NRPN_LSB,            (uint8_t) MidiProtocol::ControlChange::NrpnLsb);
        EXPECT_EQ(MIDI_NRPN_MSB,            (uint8_t) MidiProtocol::ControlChange::NrpnMsb);
        EXPECT_EQ(MIDI_RPN_LSB,             (uint8_t) MidiProtocol::ControlChange::RpnLsb);
        EXPECT_EQ(MIDI_RPN_MSB,             (uint8_t) MidiProtocol::ControlChange::RpnMsb);

        //  Channel Mode Commands
        EXPECT_EQ(MIDI_ALL_SOUND_OFF,           (uint8_t) MidiProtocol::ChannelMode::AllSoundOff);
        EXPECT_EQ(MIDI_RESET_ALL_CONTROLLERS,   (uint8_t) MidiProtocol::ChannelMode::ResetAllControllers);
        EXPECT_EQ(MIDI_LOCAL_CONTROL,           (uint8_t) MidiProtocol::ChannelMode::LocalControl);
        EXPECT_EQ(MIDI_ALL_NOTES_OFF,           (uint8_t) MidiProtocol::ChannelMode::AllNotesOff);
        EXPECT_EQ(MIDI_OMNI_OFF,                (uint8_t) MidiProtocol::ChannelMode::OmniOff);
        EXPECT_EQ(MIDI_OMNI_ON,                 (uint8_t) MidiProtocol::ChannelMode::OmniOn);
        EXPECT_EQ(MIDI_MONO_ON,                 (uint8_t) MidiProtocol::ChannelMode::Mono);
        EXPECT_EQ(MIDI_POLY_ON,                 (uint8_t) MidiProtocol::ChannelMode::Poly);
        EXPECT_EQ(MIDI_LOCAL_CONTROL_OFF,       (uint8_t) MidiProtocol::LocalControlStatus::Off);
        EXPECT_EQ(MIDI_LOCAL_CONTROL_ON,        (uint8_t) MidiProtocol::LocalControlStatus::On);

        // System Common Commands
        EXPECT_EQ(MIDI_SONG_POSITION_POINTER,   MidiProtocol::SystemCommon::SongPositionPointer);
        EXPECT_EQ(MIDI_SONG_SELECT,             MidiProtocol::SystemCommon::SongSelect);
        EXPECT_EQ(MIDI_TUNING_REQUEST,          MidiProtocol::SystemCommon::TunningRequest);

        // MTC Commands
        EXPECT_EQ(MIDI_MTC_QUARTER_FRAME,   (uint8_t) MidiProtocol_MTC::QuarterFrame);
        EXPECT_EQ(MIDI_MTC_FRAMES_LSB,      (uint8_t) MidiProtocol_MTC::TimeComponent::FramesLSB);
        EXPECT_EQ(MIDI_MTC_FRAMES_MSB,      (uint8_t) MidiProtocol_MTC::TimeComponent::FramesMSB);
        EXPECT_EQ(MIDI_MTC_SECONDS_LSB,     (uint8_t) MidiProtocol_MTC::TimeComponent::SecondsLSB);
        EXPECT_EQ(MIDI_MTC_SECONDS_MSB,     (uint8_t) MidiProtocol_MTC::TimeComponent::SecondsMSB);
        EXPECT_EQ(MIDI_MTC_MINUTES_LSB,     (uint8_t) MidiProtocol_MTC::TimeComponent::MinutesLSB);
        EXPECT_EQ(MIDI_MTC_MINUTES_MSB,     (uint8_t) MidiProtocol_MTC::TimeComponent::MinutesMSB);
        EXPECT_EQ(MIDI_MTC_HOURS_LSB,       (uint8_t) MidiProtocol_MTC::TimeComponent::HoursLSB);
        EXPECT_EQ(MIDI_MTC_HOURS_MSB,       (uint8_t) MidiProtocol_MTC::TimeComponent::HoursMSB);

        // MTC Frame Rates
        EXPECT_EQ(MIDI_MTC_FRAME_RATE_24FPS,            (uint8_t) MidiProtocol_MTC::FrameRate::FPS24);
        EXPECT_EQ(MIDI_MTC_FRAME_RATE_25FPS,            (uint8_t) MidiProtocol_MTC::FrameRate::FPS25);
        EXPECT_EQ(MIDI_MTC_FRAME_RATE_30FPS_DROP_FRAME, (uint8_t) MidiProtocol_MTC::FrameRate::FPS30DropFrame);
        EXPECT_EQ(MIDI_MTC_FRAME_RATE_30FPS,            (uint8_t) MidiProtocol_MTC::FrameRate::FPS30); 

        // Real-Time MIDI Messages
        EXPECT_EQ(MIDI_REALTIME_TIMING_TICK,    MidiProtocol_RealTime::TimingTick);
        EXPECT_EQ(MIDI_REALTIME_START,          MidiProtocol_RealTime::Start);
        EXPECT_EQ(MIDI_REALTIME_CONTINUE,       MidiProtocol_RealTime::Continue);
        EXPECT_EQ(MIDI_REALTIME_STOP,           MidiProtocol_RealTime::Stop);
        EXPECT_EQ(MIDI_REALTIME_ACTIVE_SENSING, MidiProtocol_RealTime::ActiveSensing);
        EXPECT_EQ(MIDI_REALTIME_SYSTEM_RESET,   MidiProtocol_RealTime::SystemReset);

        // MIDI Exclusive Messages
        EXPECT_EQ(MIDI_SYSEX_START, MidiProtocol_SysEx::Start);
        EXPECT_EQ(MIDI_SYSEX_END,   MidiProtocol_SysEx::End);
    }   
}