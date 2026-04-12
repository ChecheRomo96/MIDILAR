#include <MIDILAR_MidiFoundation.h>
#include <iostream>
#include <chrono>
#include <vector>

using namespace MIDILAR::MidiFoundation;

void BenchmarkSilentCallback(const uint8_t*, size_t) {
    // Do nothing
}

void BenchmarkParser(size_t messageCount, size_t messageSize) {
    std::vector<uint8_t> buffer;
    buffer.reserve(messageCount * messageSize);
    
    Message msg;
    for (size_t i = 0; i < messageCount; ++i) {
        if (messageSize == 1)
            msg.TimingTick(); // 1-byte Real-Time
        else
            msg.NoteOn(60 + (i % 12), 100, 1); // 3-byte

        buffer.insert(buffer.end(), msg.Buffer(), msg.Buffer() + msg.size());
    }

    MessageParser parser(64);
    parser.BindDefaultCallback(BenchmarkSilentCallback);

    auto start = std::chrono::high_resolution_clock::now();
    parser.ProcessData(buffer.data(), buffer.size());
    auto end = std::chrono::high_resolution_clock::now();

    auto elapsedNs = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    std::cout << "Processed " << messageCount << " messages ("
              << messageSize << " bytes each) in "
              << elapsedNs << " ns | Avg: "
              << (elapsedNs / messageCount) << " ns/message\n";
}

int main() {
    BenchmarkParser(100, 3);
    BenchmarkParser(1000, 3);
    BenchmarkParser(1000, 1);
    return 0;
}
