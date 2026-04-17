#ifndef MIDILAR_VELOCITY_SHAPER_DEVICE_H
#define MIDILAR_VELOCITY_SHAPER_DEVICE_H

    #include <MIDILAR_BuildSettings.h>
    #include <MidiFoundation/DeviceBase/DeviceBase.h>
    #include <MidiFoundation/MessageParser/MessageParser.h>
    #include <dspFoundation/LUT/Shaping/GammaLUT.h>

    namespace MIDILAR::MidiDevices {

        ///////////////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @class VelocityShaper
         * @brief A MIDI device that remaps note velocity using a nonlinear shaping LUT.
         *
         * Applies a precomputed 3D gamma-morph transformation to MIDI note velocities.
         * Morph and exponentiation gain can be controlled dynamically.
         */
        class VelocityShaper : public MIDILAR::MidiFoundation::DeviceBase {
        protected:
            MIDILAR::MidiFoundation::MessageParser _MessageParser;
            uint16_t _InputChannels;
            MIDILAR::dspFoundation::LUT::Shaping::GammaLUT<uint8_t> _LUT;

            float _CurrentMorph = 0, _CurrentGain = 0;
            uint8_t _minVel = 0, _maxVel = 127;

        private:
            void _ChannelVoiceCallback(const uint8_t* Message, size_t Size);
            void _DefaultCallback(const uint8_t* Message, size_t Size);
            float TrilinearInterpolation(uint8_t xf, float yf, float zf) const;
        
        public:
            VelocityShaper();
            bool LUT_Resize(size_t X, size_t Morph=3, size_t ExpGain=3);
            bool LUT_ResizeVel(size_t X);
            bool LUT_ResizeMorph(size_t Morph);
            bool LUT_ResizeGain(size_t ExpGain);

            void MidiInput(const uint8_t* Message, size_t Size) override;
            void SetInputChannels(uint16_t ChannelMap);
            
            void SetVelocityRange(uint8_t minVel, uint8_t maxVel);
            void SetVelocityMin(uint8_t minVel);
            void SetVelocityMax(uint8_t maxVel);
            uint8_t MinVelocity() const;
            uint8_t MaxVelocity() const;
            size_t GetVelocityDepth() const;

            void SetMorphRange(float MorphMin, float MorphMax);
            void SetMorphMin(float MorphMin);
            void SetMorphMax(float MorphMax);
            void SetMorph(float SetMorph);
            size_t GetMorphDepth() const;
            float Morph() const;
            float MorphMin() const;
            float MorphMax() const;

            void SetGainRange(float GainMin, float GainMax);
            void SetGain(float ExpGain);
            size_t GetGainDepth() const;
            float Gain() const;
            float GainMin() const;
            float GainMax() const;

            const MIDILAR::dspFoundation::LUT::Shaping::GammaLUT<uint8_t>& GetLUT() const;

        private:
            static void _ChannelVoiceCallback(void* context, const uint8_t* Data, size_t Size);
            static void _DefaultCallback(void* context, const uint8_t* Data, size_t Size);
        };

        ///////////////////////////////////////////////////////////////////////////////////////////////////////

    } // namespace MIDILAR::MidiProcessors

#endif // MIDILAR_VELOCITY_SHAPER_DEVICE_H
