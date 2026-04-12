#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <dspFoundation/LUT/LUT1D/LUT1D.h>

using namespace MIDILAR::dspFoundation::LUT;

namespace{
class RampLUT : public LUT1D<float> {
public:
    explicit RampLUT(size_t size) {
        if (SetBufferSize(size)) {
            // Eval will be triggered here
        }
    }

protected:
    void Eval() override {
        for (size_t i = 0; i < this->BufferSize(); ++i) {
            float normalized = static_cast<float>(i) / (this->BufferSize() - 1);
            this->SetData(i, normalized);
        }
    }
};
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: RampLUT <N> <output_directory>\n";
        return 1;
    }

    size_t N = static_cast<size_t>(std::stoi(argv[1]));
    const std::filesystem::path outDir(argv[2]);
    const std::filesystem::path outFile = outDir / "RampLUT.m";

    std::ofstream file(outFile);
    if (!file) {
        std::cerr << "Failed to open output file: " << outFile << "\n";
        return 1;
    }

    RampLUT lut(N);

    file << "% Auto-generated ramp LUT plot\n";
    file << "x = 0:" << (N - 1) << ";\n";
    file << "y = [";
    for (size_t i = 0; i < N; ++i) {
        file << lut.GetValue(i);
        if (i < N - 1) file << ", ";
    }
    file << "];\n";

    file << "stairs(x, y, 'k-', 'LineWidth', 1.5);\n";
    file << "title('Ramp LUT - N = " << N << "');\n";
    file << "xlabel('Index'); ylabel('Value');\n";
    file << "grid on;\n";
    file << "xlim([0 " << (N - 1) << "]);\n";
    file << "ylim([0 1]);\n";

    std::cout << "MATLAB script saved to: " << outFile << "\n";
    return 0;
}
