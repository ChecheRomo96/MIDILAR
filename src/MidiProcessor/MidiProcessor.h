#ifndef MIDILAR_MIDI_PROCESSOR_H
#define MIDILAR_MIDI_PROCESSOR_H

#if __has_include(<CPvector.h>)
    #include <CPvector.h>
#endif

#include <MIDILAR_BuildSettings.h>
#include <stdlib.h>
#include <Tools/Clock/Clock.h>

namespace MIDILAR {
    class MidiProcessor {
        
        public:
            /**
             * @brief Type definition for MIDI out event callback.
             * 
             * This typedef defines the type for the callback function used to handle MIDI output events.
             * 
             * @param Message Pointer to the MIDI message buffer.
             * @param size Size of the MIDI message buffer.
             */
            using MidiOutEventType = void (*)(uint8_t* Message, size_t size);

        protected:
            MidiOutEventType _MidiOutHandler;

        public:  
            MidiProcessor();
            MidiProcessor(MidiOutEventType MidiOutHandler);

            void MidiOutApiLink(MidiOutEventType MidiOutHandler);
            bool MidiOutApiStatus();

            #if __has_include(<CPvector.h>)
                void MidiInput(cpstd::vector<uint8_t>::iterator begin, cpstd::vector<uint8_t>::iterator end);
            #endif
            virtual void MidiInput(uint8_t* Message, size_t size);
            
            virtual void Update(Tools::Clock::ckData SystemTime);
            virtual void ClockTick();

        protected:
#           if __has_include(<CPvector.h>)
                void MidiOutput(cpstd::vector<uint8_t>::iterator begin, cpstd::vector<uint8_t>::iterator end);
            #endif
            void MidiOutput(uint8_t* Message, size_t size);

    };
}

#endif // MIDILAR_MIDI_PROCESSOR_H
