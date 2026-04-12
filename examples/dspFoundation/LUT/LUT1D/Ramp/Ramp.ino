#include <MIDILAR.h>
#include <dspFoundation/LUT/LUT1D/LUT1D.h>

using namespace MIDILAR::dspFoundation::LUT;

#define LUT_SIZE 64  // You can change this size for testing

namespace{
// Custom LUT subclass
class RampLUT : public LUT1D<float> {
public:
    RampLUT(size_t size) {
        if (SetBufferSize(size)) {
            // Eval will be called
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

void setup() {
    Serial.begin(115200);
    while (!Serial);  // Wait for serial monitor to open (optional)

    Serial.println("% Auto-generated Ramp LUT for MATLAB/Octave");
    Serial.print("x = 0:");
    Serial.print(LUT_SIZE - 1);
    Serial.println(";");

    Serial.print("y = [");

    RampLUT lut(LUT_SIZE);

    for (size_t i = 0; i < LUT_SIZE; ++i) {
        Serial.print(lut.GetValue(i), 6);  // Print with 6 decimal digits
        if (i < LUT_SIZE - 1) Serial.print(", ");
    }

    Serial.println("];");

    Serial.println("stairs(x, y, 'k-', 'LineWidth', 1.5);");
    Serial.println("title('Ramp LUT - Arduino');");
    Serial.println("xlabel('Index'); ylabel('Value');");
    Serial.println("grid on;");
    Serial.print("xlim([0 ");
    Serial.print(LUT_SIZE - 1);
    Serial.println("]);");
    Serial.println("ylim([0 1]);");
}

void loop() {
    // Do nothing — single-shot export
}
