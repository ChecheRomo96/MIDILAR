#include <iostream>
#include <fstream>
#include <filesystem>
#include <cmath>
#include <string>
#include <dspFoundation/LUT/LUT1D/LUT1D.h>
#include <dspFoundation/Generators/Periodic/Periodic.h>

using namespace MIDILAR::dspFoundation::LUT;
using namespace MIDILAR::dspFoundation::Generators::Periodic;
 
namespace {
template<typename T>
class WaveformGenerator : public LUT1D<T> {
    private:
        float amplitude;
        float dcOffset;
        float phaseOffset;
        uint8_t waveform;

    public:
        WaveformGenerator(size_t size, uint8_t Waveform, float amp, float offset, float phase)
        : amplitude(amp), dcOffset(offset), phaseOffset(phase) {
            this->SetBufferSize(size); // Allocates and triggers Eval()
            SetWaveform(Waveform);
        }

        void SetWaveform(uint8_t Wave){
            if(Wave<=3){
                waveform=Wave;
                Eval();
            }
        }

    protected:
        void Eval() override {
            switch(waveform){
                case 0: Sine<T>(this->GetBuffer(), this->BufferSize(),amplitude, dcOffset, phaseOffset); break;
                case 1: Saw<T>(this->GetBuffer(), this->BufferSize(),amplitude, dcOffset, phaseOffset); break;
                case 2: Square<T>(this->GetBuffer(), this->BufferSize(),amplitude, dcOffset, phaseOffset); break;
                case 3: Triangle<T>(this->GetBuffer(), this->BufferSize(),amplitude, dcOffset, phaseOffset); break;
                default: break;
            }
        }
    };
}
 


int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: WaveformGenerator <N> <output_directory>\n";
        return 1;
    }

    const std::filesystem::path outDir(argv[2]);
    const std::filesystem::path outFile = outDir / "WaveformPlot.m";

    size_t N = static_cast<size_t>(std::stoi(argv[1]));
    const char* waveformNames[] = { "Sine", "Sawtooth", "Square", "Triangle" };
    const char* colors[] = { "b", "r", "g", "m" };

    std::ofstream file(outFile);
    if (!file) {
        std::cerr << "Failed to open output file: " << outFile << "\n";
        return 1;
    }

    file << "% Auto-generated waveform plot from WaveformGenerator\n";
    file << "x = 0:" << (N - 1) << ";\n";

    WaveformGenerator<float> lut(N, 0, 1.0f, 0.0f, 0.0f);

    for (int i = 0; i < 4; ++i) {
        lut.SetWaveform(i);

        file << "y" << i << " = [";
        for (size_t j = 0; j < N; ++j) {
            file << lut.GetValue(j);
            if (j < N - 1) file << ", ";
        }
        file << "];\n";
        file << "stairs(x, y" << i << ", '" << colors[i] << "-', 'DisplayName', '" << waveformNames[i] << "'); hold on;\n";
    }

    file << "hold off;\n";
    file << "title('Waveform Comparison - N = "<<N<<"');\n";
    file << "xlabel('Phase'); ylabel('Amplitude');\n";
    file << "legend show;\n";
    file << "xlim([0,"<<N-1<<"]);\n";
    file << "grid on;\n";

    std::cout << " MATLAB script saved to: " << outFile << "\n";
    return 0;
}
