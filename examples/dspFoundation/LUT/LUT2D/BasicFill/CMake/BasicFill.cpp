
#include <iostream>
#include <dspFoundation/LUT/LUT2D.h>
using namespace MIDILAR::dspFoundation::LUT;

int main() {
    LUT2D<float> lut(4,4);
    for(size_t y=0;y<lut.Height();++y){
        for(size_t x=0;x<lut.Width();++x){
            lut.SetBinValue(x,y,(float)(x+y));
        }
    }
    std::cout<<"BasicFill\n";
    for(size_t y=0;y<lut.Height();++y){
        for(size_t x=0;x<lut.Width();++x){
            std::cout<<lut.GetValue(x,y)<<" ";
        }
        std::cout<<"\n";
    }
}
