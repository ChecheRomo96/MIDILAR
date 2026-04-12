#include <MIDILAR.h>
#include <dspFoundation/LUT/LUT1D/LUT1D.h>
#include <math.h>  // Needed for log()

using namespace MIDILAR::dspFoundation::LUT;

#define LUT_SIZE 64
#define LOG_BASE 10.0f

namespace{
class LogCompressorLUT : public LUT1D<float> {
public:
    LogCompressorLUT(size_t size, float logBase = LOG_BASE)
        : base(logBase) {
        if (SetBufferSize(size)) {
            // Eval will be called
        }
    }

protected:
    float base;

    void Eval() override {
        for (size_t i = 0; i < this->BufferSize(); ++i) {
            float x = static_cast<float>(i) / (this->BufferSize() - 1); // [0, 1]
            float y = (base > 1.0f)
                ? (1.0f - logf(1.0f + (base - 1.0f) * (1.0f - x)) / logf(base))
                : 1.0f - x;
            this->SetData(i, y);
        }
    }
};
}

void setup() {
    Serial.begin(115200);
    while (!Serial);  // Optional wait for Serial Monitor

    LogCompressorLUT lut(LUT_SIZE);

    Serial.println("% Auto-generated logarithmic compressor LUT");
    Serial.print("x = 0:");
    Serial.print(LUT_SIZE - 1);
    Serial.println(";");

    Serial.print("y = [");
    for (size_t i = 0; i < LUT_SIZE; ++i) {
        Serial.print(lut.GetValue(i), 6);
        if (i < LUT_SIZE - 1) Serial.print(", ");
    }
    Serial.println("];");

    Serial.println("stairs(x, y, 'r-', 'LineWidth', 2);");
    Serial.print("title('Logarithmic Compressor LUT, N = ");
    Serial.print(LUT_SIZE);
    Serial.println("');");
    Serial.println("xlabel('Index'); ylabel('Output'); grid on;");
    Serial.print("xlim([0 ");
    Serial.print(LUT_SIZE - 1);
    Serial.println("]);");
    Serial.println("ylim([0 1]);");
}

void loop() {
    // No real-time action needed
}
