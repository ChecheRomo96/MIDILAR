#include <MIDILAR_MidiFoundation.h>
#include <stdint.h>
#include <iostream>
#include <vector>  // Required for std::vector

// **Default Callback Function**
// This function will be called when a MIDI message is processed and no specific handler is set.
void DefaultCallback(const uint8_t* Data, size_t Size) {
    std::cout << "Default Callback Invoked -> ";

    // Print the raw MIDI message in a readable format
    for (size_t i = 0; i < Size; i++) {
        std::cout << std::to_string(Data[i]);
        if (i < Size - 1) {
            std::cout << ", ";
        }
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
    // **Create a MessageParser with a buffer size of 256**
    // This will handle and process incoming MIDI data.
    MIDILAR::MidiFoundation::MessageParser myDecoder(256);

    // **Set the Default Callback**
    // This ensures that if a message does not have a specific callback, it will be handled here.
    myDecoder.BindDefaultCallback(DefaultCallback);

    // **Simulating Split MIDI Data Processing**
    // In real-world MIDI streams, messages might not always arrive as complete packets.
    // Here, we split the buffer into two chunks to test if the parser can handle partial messages.
    
    myDecoder.ProcessData(Buffer.data(), 2);  // Send first 2 bytes (incomplete message)
    myDecoder.ProcessData(Buffer.data() + 2, Buffer.size() - 2);  // Send the remaining bytes

    return 0;
}
