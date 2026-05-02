
#include <MIDILAR.h>
using namespace MIDILAR::DspCore::LUT;

LUT2D<float> lut(8,8);

void setup(){
    Serial.begin(115200);
    Serial.println("FunctionSurface");
}
void loop(){}
