
#include <MIDILAR.h>
using namespace MIDILAR::DspCore::LUT;

LUT2D<float> lut(4,4);

void setup(){
    Serial.begin(115200);
    for(size_t y=0;y<lut.Height();++y){
        for(size_t x=0;x<lut.Width();++x){
            lut.SetBinValue(x,y,x+y);
        }
    }
    Serial.println("BasicFill");
}
void loop(){}
