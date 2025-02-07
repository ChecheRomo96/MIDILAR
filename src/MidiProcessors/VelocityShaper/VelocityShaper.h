#ifndef MIDILAR_VELOCITY_SHAPER_H
#define MIDILAR_VELOCITY_SHAPER_H

#include <MidiFoundation/Processor/Processor.h>
#include <MidiFoundation/MessageParser/MessageParser.h>
#include <AudioFoundation/LUT/LogExpLUT.h>

namespace MIDILAR::MidiProcessors {

	///////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @class VelocityShaper
     * @brief A MIDI processor that remaps velocity using a nonlinear shaping function.
     *
     * This processor applies a lookup table transformation (e.g., LogExpLUT) to MIDI note velocities.
     */
	    class VelocityShaper : public MIDILAR::MidiFoundation::Processor {
	    protected:
			MIDILAR::MidiFoundation::MessageParser _MessageParser;
	        uint16_t _InputChannels;
	        MIDILAR::AudioFoundation::LUT::LogExpLUT<uint8_t, uint8_t> _LUT;

	    private:
	        void _ChannelVoiceCallback(const MidiFoundation::Message& Data);
	        void _DefaultCallback(const MidiFoundation::Message& Data);

	    public:

	        VelocityShaper();

	        ///////////////////////////////////////////////////////////////////////////////////////////////
	        /**
	         * @brief Processes incoming MIDI messages.
	         * @param Message Pointer to the MIDI message buffer.
	         * @param Size The size of the incoming message.
	         */
	        	void MidiInput(const uint8_t* Message, size_t Size) override;
	        //
	        ///////////////////////////////////////////////////////////////////////////////////////////////


	        	void SetMorph(float SetMorph);
	        	float Morph() const;

	        	void SetExpGain(float ExpGain);
	        	float ExpGain() const;
	    };
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace MIDILAR::MidiProcessors

#endif // MIDILAR_VELOCITY_SHAPER_H
