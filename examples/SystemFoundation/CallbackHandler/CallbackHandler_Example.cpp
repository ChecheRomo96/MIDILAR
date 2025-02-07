#include <iostream>
#include <chrono>
#include <cstdint>
#include <SystemFoundation/CallbackHandler/CallbackHandler.h>

using namespace MIDILAR::SystemFoundation;

CallbackHandler<uint8_t, uint8_t>   CallbackA;
CallbackHandler<void, uint8_t>      CallbackB;
CallbackHandler<uint8_t, void>      CallbackC;
CallbackHandler<void>               CallbackD;  // No Return nor Params


uint8_t Val = 0;

uint8_t CallbackHelperA(uint8_t x){
    std::cout<<(int)x<<std::endl;
    Val = x;
    return Val;
}

void CallbackHelperB(uint8_t x){
    std::cout<<(int)x<<std::endl;
    Val = x;
}

uint8_t CallbackHelperC(){
    std::cout<<(int)Val++<<std::endl;
    return Val;
}

void CallbackHelperD(){
    std::cout<<(int)Val<<std::endl;
}

int main() {
    
    CallbackA.bind(CallbackHelperA);
    CallbackB.bind(CallbackHelperB);
    CallbackC.bind(CallbackHelperC);
    CallbackD.bind(CallbackHelperD);

    uint8_t data;
    for(uint8_t i = 0; i < UINT8_MAX ; i++){
        data = CallbackA.invoke(i);
        std::cout<<(int)i<<" - "<<(int)data<<std::endl;
    }

    for(uint8_t i = 0; i < UINT8_MAX ; i++){
        CallbackB.invoke(i);
    }



    return 0;
}
