#include "Enums_MTC.h"

using namespace MidiProtocol_MTC;

uint8_t SongPosition::PositionData::GetData(const MidiProtocol_MTC::TimeComponent& TC) const {
    switch (TC) {
        case TimeComponent::FramesLSB:  return Frames & 0x0F;
        case TimeComponent::FramesMSB:  return (Frames >> 4) & 0x0F;
        case TimeComponent::SecondsLSB: return Seconds & 0x0F;
        case TimeComponent::SecondsMSB: return (Seconds >> 4) & 0x0F;
        case TimeComponent::MinutesLSB: return Minutes & 0x0F;
        case TimeComponent::MinutesMSB: return (Minutes >> 4) & 0x0F;
        case TimeComponent::HoursMSB:   return Hours & 0x0F;
        case TimeComponent::HoursLSB:   return ((Hours >> 4) & 0b1) | ((FrameRate & 0b11) << 1);
        default:                        return 0; // Return 0 for invalid TimeComponent
    }
}

bool SongPosition::PositionData::isDataValid() const {
    switch(static_cast<MidiProtocol_MTC::FrameRate>(FrameRate)){
        case MidiProtocol_MTC::FrameRate::FPS24: 
                if(Frames >= 24){return 0;}
            break;
        case MidiProtocol_MTC::FrameRate::FPS25: 
                if(Frames >= 25){return 0;}
            break;
        case MidiProtocol_MTC::FrameRate::FPS30DropFrame: 
                if(Frames >= 30){return 0;}
                if((Seconds == 0) && ((Minutes%10) == 0) && (Frames == 0 || Frames == 1)){return 0;}
            break;
        case MidiProtocol_MTC::FrameRate::FPS30: 
                if(Frames >= 30){return 0;}
            break;
        default: return 0;
    }

    if(Seconds >= 60){return 0;}
    if(Minutes >= 60){return 0;}
    if(Hours >= 32){return 0;}
    return 1;
}

void SongPosition::PositionData::IncreaseFrame(){
    Frames++;

    // Check for frame overflow and handle appropriately
    if (Frames == GetFrameLimit()) {
        if (static_cast<MidiProtocol_MTC::FrameRate>(FrameRate) == MidiProtocol_MTC::FrameRate::FPS30DropFrame) {
            if (Seconds == 59 && (Minutes % 10) != 9) {
                Frames = 0;  // Reset frames for non-multiples of 10
            } 
            else {
                Frames = 2;  // Skip frames 00 and 01
            }
            IncrementSeconds();
        } 
        else {
            Frames = 0;  // Reset frames
            IncrementSeconds();  // Increment seconds normally
        }
    }
}

uint8_t SongPosition::PositionData::GetFrameLimit() const {
    switch (static_cast<MidiProtocol_MTC::FrameRate>(FrameRate)) {
        case MidiProtocol_MTC::FrameRate::FPS24: return 24;
        case MidiProtocol_MTC::FrameRate::FPS25: return 25;
        case MidiProtocol_MTC::FrameRate::FPS30: return 30;
        case MidiProtocol_MTC::FrameRate::FPS30DropFrame: return 30;
        default: return 30; // Default to 30 fps if frame rate is invalid
    }
}

void SongPosition::PositionData::IncrementSeconds() {
    Seconds++;
    if (Seconds == 60) {
        Seconds = 0;
        Minutes++;
        if (Minutes == 60) {
            Minutes = 0;
            Hours++;
            if (Hours == 24) { // Optional: wrap around after 24 hours
                Hours = 0;
            }
        }
    }
}

void SongPosition::SetPosition(const PositionData& data) {
    if (!data.isDataValid()) {
        return;
    }
    _position = data;
}

// Get the current position data
const SongPosition::PositionData& SongPosition::GetPosition() const {
    return _position;
}

// Increment the frame in the current position
void SongPosition::IncrementFrame() {
    _position.IncreaseFrame();
}