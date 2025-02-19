#ifndef MIDILAR_VELOCITY_SHAPER_H
#define MIDILAR_VELOCITY_SHAPER_H

    #include <System/BuildSettings.h>
	#include <MidiFoundation/Processor/Processor.h>
	#include <MidiFoundation/MessageParser/MessageParser.h>
	#include <MIDILAR_dspFoundation.h>

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
		        //MIDILAR::dspFoundation::LUT::LogExpLUT<uint8_t, uint8_t> _LUT;

		    private:
		        void _ChannelVoiceCallback(const uint8_t* Message, size_t Size);
		        void _DefaultCallback(const uint8_t* Message, size_t Size);

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

					void SetInputChannels(uint16_t ChannelMap);

		        	void SetMorph(float SetMorph);
		        	float Morph() const;

		        	void SetExpGain(float ExpGain);
		        	float ExpGain() const;

		    private:
		    	
	    		static void StaticChannelVoiceCallback(void* context, const uint8_t* Data, size_t Size);
	    		static void StaticDefaultCallback(void* context, const uint8_t* Data, size_t Size);
		    };
		//
		///////////////////////////////////////////////////////////////////////////////////////////////////////

	} // namespace MIDILAR::MidiProcessors

#endif // MIDILAR_VELOCITY_SHAPER_H
