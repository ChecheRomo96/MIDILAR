#include <MIDILAR.h>
#include <stdint.h>
#include <iostream>
#include <vector>

// ===============================
// CALLBACK FUNCTIONS
// ===============================
// Each function below is a callback that handles a specific type of MIDI message. 
// These functions print the message type along with its data when called.

/* Handles Channel Voice messages such as Note On, Note Off, Aftertouch, etc. */
void ChannelVoiceCallback(const uint8_t* Data, size_t Size) {
    std::cout << "[Channel Voice] ";
    for (size_t i = 0; i < Size; i++) {
        std::cout << std::to_string(Data[i]) << " ";
    }
    std::cout << std::endl;
}

/* Handles Control Change messages (e.g., Mod Wheel, Volume). */
void ControlChangeCallback(const uint8_t* Data, size_t Size) {
    std::cout << "[Control Change] ";
    for (size_t i = 0; i < Size; i++) {
        std::cout << std::to_string(Data[i]) << " ";
    }
    std::cout << std::endl;
}

/* Handles Real-Time messages such as Timing Clock and Active Sensing. */
void RealTimeCallback(const uint8_t* Data, size_t Size) {
    std::cout << "[Real-Time] ";
    for (size_t i = 0; i < Size; i++) {
        std::cout << std::to_string(Data[i]) << " ";
    }
    std::cout << std::endl;
}

/* Handles System Common messages such as Song Select and Tuning Request. */
void SystemCommonCallback(const uint8_t* Data, size_t Size) {
    std::cout << "[System Common] ";
    for (size_t i = 0; i < Size; i++) {
        std::cout << std::to_string(Data[i]) << " ";
    }
    std::cout << std::endl;
}

/* Handles System Exclusive (SysEx) messages. */
void SysExCallback(const uint8_t* Data, size_t Size) {
    std::cout << "[System Exclusive] ";
    for (size_t i = 0; i < Size; i++) {
        std::cout << std::to_string(Data[i]) << " ";
    }
    std::cout << std::endl;
}

/* Handles MIDI Time Code (MTC) messages. */
void MTCCallback(const uint8_t* Data, size_t Size) {
    std::cout << "[MIDI Time Code] ";
    for (size_t i = 0; i < Size; i++) {
        std::cout << std::to_string(Data[i]) << " ";
    }
    std::cout << std::endl;
}

/* Handles MIDI Show Control (MSC) messages. */
void MSCCallback(const uint8_t* Data, size_t Size) {
    std::cout << "[MIDI Show Control] ";
    for (size_t i = 0; i < Size; i++) {
        std::cout << std::to_string(Data[i]) << " ";
    }
    std::cout << std::endl;
}

/* Default callback for unhandled messages. */
void DefaultCallback(const uint8_t* Data, size_t Size) {
    std::cout << "[Default Callback] ";
    for (size_t i = 0; i < Size; i++) {
        std::cout << std::to_string(Data[i]) << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char *argv[]) {

    // ===============================
    // CREATING MIDI MESSAGES
    // ===============================
    // The vector 'Buffer' stores multiple MIDI messages, which will be processed later.
    std::vector<uint8_t> Buffer;
    MIDILAR::MidiFoundation::Message msg;

    // **Create and store a Note On MIDI message**
    msg.NoteOn(60, 120, 1);  // NoteOn: Note 60 (Middle C), Velocity 120, Channel 1
    Buffer.insert(Buffer.end(), msg.Buffer(), msg.Buffer() + msg.size());

    // **Create and store a Note Off MIDI message**
    msg.NoteOff(60, 120, 1);  // NoteOff: Same note, same velocity, same channel
    Buffer.insert(Buffer.end(), msg.Buffer(), msg.Buffer() + msg.size());

    // **Create and store a Control Change message (e.g., Mod Wheel)**
    msg.ControlChange(1, 64, 1);  // Control Change: CC 1 (Modulation), Value 64, Channel 1
    Buffer.insert(Buffer.end(), msg.Buffer(), msg.Buffer() + msg.size());

    // **Create and store a Real-Time Timing Clock message**
    msg.TimingTick();  // Used for MIDI clock synchronization
    Buffer.insert(Buffer.end(), msg.Buffer(), msg.Buffer() + msg.size());

    // **Create and store a System Common Song Select message**
    msg.SongSelect(3);  // Select Song number 3
    Buffer.insert(Buffer.end(), msg.Buffer(), msg.Buffer() + msg.size());

    // **Create and store a MIDI Time Code Quarter Frame message**
    msg.MTC_QuarterFrame(2, 0x3F);
    Buffer.insert(Buffer.end(), msg.Buffer(), msg.Buffer() + msg.size());

    // **Create and store a System Exclusive (SysEx) message**
    uint8_t sysExData[] = { 0x7D, 0x01, 0x02, 0x03 };  // Example SysEx payload
    msg.SystemExclusive(sysExData, sizeof(sysExData));
    Buffer.insert(Buffer.end(), msg.Buffer(), msg.Buffer() + msg.size());

    /*
    // Uncomment to add a MIDI Show Control (MSC) message
    msg.ShowControl();
    Buffer.insert(Buffer.end(), msg.Buffer(), msg.Buffer() + msg.size());
    */

    // ===============================
    // SETTING UP MIDI PARSER
    // ===============================
    MIDILAR::MidiFoundation::MessageParser myDecoder(256);

    // ===============================
    // TEST 1: Using Default Callback Only
    // ===============================
    std::cout << "\n---- TEST 1: Default Callback Only ----\n";
    myDecoder.BindDefaultCallback(DefaultCallback);
    myDecoder.ProcessData(Buffer.data(), Buffer.size());
    myDecoder.UnbindAll();

    // ===============================
    // TEST 2: Using Specific Callbacks
    // ===============================
    std::cout << "\n---- TEST 2: Specific Callbacks ----\n";
    myDecoder.BindDefaultCallback(DefaultCallback);
    myDecoder.BindChannelVoiceCallback(ChannelVoiceCallback);
    myDecoder.BindControlChangeCallback(ControlChangeCallback);
    myDecoder.BindRealTimeCallback(RealTimeCallback);
    myDecoder.BindSystemCommonCallback(SystemCommonCallback);
    myDecoder.BindSysExCallback(SysExCallback);
    myDecoder.BindMTCCallback(MTCCallback);
    myDecoder.BindMSCCallback(MSCCallback);
    myDecoder.ProcessData(Buffer.data(), Buffer.size());
    myDecoder.UnbindAll();

    // ===============================
    // TEST 3: No Default, Only Channel Voice Callback
    // ===============================
    std::cout << "\n---- TEST 3: No Default, Only Channel Voice Callbacks ----\n";
    myDecoder.BindChannelVoiceCallback(ChannelVoiceCallback);
    myDecoder.ProcessData(Buffer.data(), Buffer.size());
    myDecoder.UnbindAll();

    // ===============================
    // TEST 4: No Default, Only Control Change Callback
    // ===============================
    std::cout << "\n---- TEST 4: No Default, Only Control Change Callbacks ----\n";
    myDecoder.BindControlChangeCallback(ControlChangeCallback);
    myDecoder.ProcessData(Buffer.data(), Buffer.size());

    std::cout << std::endl;
    
    return 0;
}
