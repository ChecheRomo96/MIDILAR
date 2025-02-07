#include <MIDILAR.h>
#include <stdint.h>
#include <iostream>
#include <vector>  // Required for std::vector

// **Default Callback Function**
// This function will be called when a MIDI message is processed and no specific handler is set.
void DefaultCallback(uint8_t* Data, size_t Size) {
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

    // **MIDI Message Buffer**
    // This vector will store multiple MIDI messages to simulate real-world incoming data.
    std::vector<uint8_t> Buffer;

    // **Create and Store a NoteOn MIDI Message**
    // NoteOn message: Key 60 (Middle C), Velocity 120, Channel 1
    MIDILAR::MidiFoundation::Message myMessage;
    myMessage.NoteOn(60, 120, 1);
    Buffer.insert(Buffer.end(), myMessage.Buffer(), myMessage.Buffer() + myMessage.size());

    // **Create and Store a NoteOff MIDI Message**
    // NoteOff message: Key 60 (Middle C), Velocity 120, Channel 1
    myMessage.NoteOff(60, 120, 1);
    Buffer.insert(Buffer.end(), myMessage.Buffer(), myMessage.Buffer() + myMessage.size());

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
