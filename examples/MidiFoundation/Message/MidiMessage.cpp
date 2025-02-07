#include <MIDILAR.h>
#include <stdint.h>
#include <iostream>

int main(int argc, char *argv[]){

    MIDILAR::MidiFoundation::Message myMessage;
    myMessage.NoteOn(60,120,1);
    std::cout<<"NoteOn Message: ";
    
    for(size_t i = 0; i < myMessage.size(); i++){
        std::cout<< std::to_string(myMessage.Buffer()[i]);
        if(i < myMessage.size()-1 ){
            std::cout<<", ";
        }
    }

    std::cout<<std::endl;
}