
#include <iostream>
#include <dspFoundation/LUT/LUT2D.h>
using namespace MIDILAR::dspFoundation::LUT;

int main(){
    LUT2D<float> lut(8,8);
    for(size_t y=0;y<lut.Height();++y){
        for(size_t x=0;x<lut.Width();++x){
            float fx=(float)x/(lut.Width()-1);
            float fy=(float)y/(lut.Height()-1);
            lut.SetBinValue(x,y,fx*fy);
        }
    }
    std::cout<<"FunctionSurface\n";

	for (size_t y = 0;y < lut.Height();++y) {
        for(size_t x=0;x<lut.Width();++x){
            std::cout<<lut.GetValue(x,y)<<" ";
        }
        std::cout<<"\n";
    }
}
