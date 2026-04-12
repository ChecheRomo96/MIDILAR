#include <MIDILAR_MidiFoundation.h>

using namespace MIDILAR::MidiFoundation;

void BenchmarkSilentCallback(const uint8_t*, size_t) {
    // Do nothing
}

void BenchmarkParser(size_t messageCount, size_t messageSize) {
    const size_t maxSize = messageCount * messageSize;
    uint8_t buffer[maxSize];
    size_t offset = 0;

    Message msg;

    for (size_t i = 0; i < messageCount; ++i) {
        if (messageSize == 1)
            msg.TimingTick();
        else
            msg.NoteOn(60 + (i % 12), 100, 1);

        const uint8_t* data = msg.Buffer();
        size_t size = msg.size();

        if (offset + size <= maxSize) {
            memcpy(buffer + offset, data, size);
            offset += size;
        }
    }

    MessageParser parser(64);
    parser.BindDefaultCallback(BenchmarkSilentCallback);

    unsigned long start = micros();
    parser.ProcessData(buffer, offset);
    unsigned long end = micros();

    Serial.print("Processed ");
    Serial.print(messageCount);
    Serial.print(" messages (");
    Serial.print(messageSize);
    Serial.print(" bytes each) in ");
    Serial.print(end - start);
    Serial.print(" us | Avg: ");
    Serial.print((end - start) / messageCount);
    Serial.println(" us/message");
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Starting MIDI Parser Benchmark...");

    BenchmarkParser(100, 3);
    BenchmarkParser(1000, 3);
    BenchmarkParser(1000, 1);

    Serial.println("All tests completed.");
}

void loop() {
    // No operation
}
