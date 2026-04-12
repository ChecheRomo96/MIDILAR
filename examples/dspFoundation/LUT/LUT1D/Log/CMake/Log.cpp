#include <dspFoundation/LUT/LUT1D/LUT1D.h>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <cmath>

using namespace MIDILAR::dspFoundation::LUT;

namespace{
class LogCompressorLUT : public LUT1D<float> {
public:
    explicit LogCompressorLUT(size_t size, float logBase = 10.0f)
        : base(logBase) {
        if (SetBufferSize(size)) {
            // Eval will be called
        }
    }

protected:
    float base;

    void Eval() override {
        for (size_t i = 0; i < this->BufferSize(); ++i) {
            float x = static_cast<float>(i) / (this->BufferSize() - 1); // [0,1]
            float y = (base > 1.0f) ? (1.0 - std::log(1 + (base - 1) * (1.0 - x)) / std::log(base)) : 1-x;
            this->SetData(i, y); // Log curve from 0 to 1
        }
    }
};
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: LogCompressorLUT <N> <output_directory>\n";
        return 1;
    }

    size_t N = static_cast<size_t>(std::stoi(argv[1]));
    std::filesystem::path outDir(argv[2]);
    std::filesystem::path outFile = outDir / "LogCompressorLUT.m";

    std::ofstream file(outFile);
    if (!file) {
        std::cerr << "Could not open output file.\n";
        return 1;
    }

    LogCompressorLUT lut(N);

    file << "% Auto-generated logarithmic LUT , N = "<<N<<"\n";
    file << "x = 0:" << (N - 1) << ";\n";
    file << "y = [";
    for (size_t i = 0; i < N; ++i) {
        file << lut.GetValue(i);
        if (i < N - 1) file << ", ";
    }
    file << "];\n";
    file << "stairs(x, y, 'r-', 'LineWidth', 2);\n";
    file << "title('Logarithmic LUT , N = "<<N<<"');\n";
    file << "xlabel('Index'); ylabel('Output'); grid on;\n";
    file << "xlim([0, " << N - 1 << "]); ylim([0, 1]);\n";

    std::cout << "MATLAB script saved to: " << outFile << "\n";
    return 0;
}
