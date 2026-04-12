#include <math.h>
#include <dspFoundation/LUT/LUT1D/LUT1D.h>  // Adjust include path to your project
#include <dspFoundation/Generators/Periodic.h>  // Adjust include path to your project

using namespace MIDILAR::dspFoundation::LUT;
 
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
        : amplitude(amp), dcOffset(offset), phaseOffset(phase)
        {
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

                case 0:
                    Periodic::SineGenerator<T>(
                        this->GetBuffer(), this->BufferSize(),
                        amplitude, dcOffset, phaseOffset
                    );
                    break;
                    
                case 1:
                    Periodic::SawtoothGenerator<T>(
                        this->GetBuffer(), this->BufferSize(),
                        amplitude, dcOffset, phaseOffset
                    ); 
                    break;
                
                case 2:
                    Periodic::SquareGenerator<T>(
                        this->GetBuffer(), this->BufferSize(),
                        amplitude, dcOffset, phaseOffset
                    );
                    break;
                    
                case 3:
                Periodic::TriangleGenerator<T>(
                    this->GetBuffer(), this->BufferSize(),
                    amplitude, dcOffset, phaseOffset
                );
                break;
                
                default: break;
            }
        }
    };
} 
 

void setup() {
    Serial.begin(115200);
    while (!Serial); // Wait for serial port to be ready

    constexpr size_t N = 32;
    WaveformGenerator<float> lut(N, 0, 1.0f, 0.0f, 0.0f);  // Generate sine LUT

    Serial.println(F("y = ["));
    for (size_t i = 0; i < N; ++i) {
        Serial.print(lut.GetValue(i), 6);
        if (i < N - 1) Serial.print(F(", "));
    }
    Serial.println(F("];"));

    Serial.print(F("x = linspace(0, 1, ")); Serial.print(N); Serial.println(F(");"));
    Serial.println(F("plot(x, y, '-o');"));
    Serial.println(F("title('Sine LUT');"));
    Serial.println(F("xlabel('Phase'); ylabel('Amplitude');"));
}

void loop() {
    // Nothing to do in loop
}
