#include "VelocityShaper.h"
#include <dspFoundation/Interpolators/Interpolators.h>

namespace MIDILAR::MidiDevices {

    VelocityShaper::VelocityShaper()
        : MIDILAR::MidiFoundation::DeviceBase(),
          _InputChannels(0xFFFF),
          _CurrentMorph(0),
          _CurrentGain(0),
          _minVel(0),
          _maxVel(127) {

        _MessageParser.BindChannelVoiceCallback<VelocityShaper, &VelocityShaper::_ChannelVoiceCallback>(this);
        _MessageParser.BindDefaultCallback<VelocityShaper, &VelocityShaper::_DefaultCallback>(this);

        _capabilities = static_cast<uint32_t>(
            MIDILAR::MidiFoundation::DeviceBase::Capabilities::MidiIn |
            MIDILAR::MidiFoundation::DeviceBase::Capabilities::MidiOut
        );

        _LUT.SetGainRange(1.0f, 10.0f);
        _LUT.SetMorphRange(-1.0f, 1.0f);
        _LUT.SetOutputGain(UINT8_MAX);
        LUT_Resize(3, 3, 3);
    }

    void VelocityShaper::MidiInput(const uint8_t* Message, size_t Size) {
        _MessageParser.ProcessData(Message, Size);
    }

    float VelocityShaper::TrilinearInterpolation(uint8_t xf, float morph, float gain) const {

        const size_t xMax = _LUT.Width()  - 1;
        const size_t yMax = _LUT.Height() - 1;
        const size_t zMax = _LUT.Depth()  - 1;

        float xNorm = (static_cast<float>(xf) / 127.0f) * xMax;
        float yNorm = (morph - _LUT.MorphMin()) / (_LUT.MorphMax() - _LUT.MorphMin()) * yMax;
        float zNorm = (gain - _LUT.GainMin()) / (_LUT.GainMax() - _LUT.GainMin()) * zMax;
        
        if (xNorm < 0.0f) xNorm = 0.0f; else if (xNorm > (float)xMax) xNorm = (float)xMax;
        if (yNorm < 0.0f) yNorm = 0.0f; else if (yNorm > (float)yMax) yNorm = (float)yMax;
        if (zNorm < 0.0f) zNorm = 0.0f; else if (zNorm > (float)zMax) zNorm = (float)zMax;

        size_t x0 = (size_t)xNorm;
        size_t y0 = (size_t)yNorm;
        size_t z0 = (size_t)zNorm;

        size_t x1 = (x0 + 1 > xMax) ? xMax : x0 + 1;
        size_t y1 = (y0 + 1 > yMax) ? yMax : y0 + 1;
        size_t z1 = (z0 + 1 > zMax) ? zMax : z0 + 1;

        float morphRange = _LUT.MorphMax() - _LUT.MorphMin();
        float gainRange  = _LUT.GainMax()  - _LUT.GainMin();

        return MIDILAR::dspFoundation::Interpolation::Trilinear<float>(
            0, 1, 0, 1, 0, 1,
            _LUT.GetValueFloat(x0, y0, z0), _LUT.GetValueFloat(x1, y0, z0),
            _LUT.GetValueFloat(x0, y1, z0), _LUT.GetValueFloat(x1, y1, z0),
            _LUT.GetValueFloat(x0, y0, z1), _LUT.GetValueFloat(x1, y0, z1),
            _LUT.GetValueFloat(x0, y1, z1), _LUT.GetValueFloat(x1, y1, z1),
            xNorm - x0, yNorm - y0, zNorm - z0
        );
    }

    void VelocityShaper::_ChannelVoiceCallback(const uint8_t* Message, size_t Size) {
        if (Size != 3) {
            MidiOutput(Message, Size);
            return;
        }

        const uint8_t status = Message[0] & 0xF0;
        const uint8_t channel = Message[0] & 0x0F;

        if (status != MIDI_NOTE_ON && status != MIDI_NOTE_OFF) {
            MidiOutput(Message, Size);
            return;
        }

        if (((_InputChannels >> channel) & 0x01) == 0) {
            MidiOutput(Message, Size);
            return;
        }

        const uint8_t velocity = Message[2];
        if (velocity == 0 || velocity == 127) {
            MidiOutput(Message, Size);
            return;
        }

        uint8_t shapedVelocity = _minVel + ((_maxVel - _minVel) * TrilinearInterpolation(
            velocity,
            Morph(),
            Gain()
        ));

        if (shapedVelocity == 0 && status == MIDI_NOTE_ON) {
            shapedVelocity = 1;
        }

        uint8_t messageOut[3] = {
            Message[0],
            Message[1],
            shapedVelocity
        };

        MidiOutput(messageOut, 3);
    }

    void VelocityShaper::_DefaultCallback(const uint8_t* Message, size_t Size) {
        MidiOutput(Message, Size);
    }

    void VelocityShaper::SetInputChannels(uint16_t ChannelMap) {
        _InputChannels = ChannelMap;
    }

    void VelocityShaper::SetMorph(float MorphValue) {
        float min = _LUT.MorphMin();
        float max = _LUT.MorphMax();
        size_t depth = GetMorphDepth();

        if (MorphValue < min) MorphValue = min;
        else if (MorphValue > max) MorphValue = max;

        _CurrentMorph = MorphValue;
    }

    float VelocityShaper::Morph() const {
        return _CurrentMorph;
    }

    void VelocityShaper::SetMorphRange(float MorphMin, float MorphMax) {
        _LUT.SetMorphRange(MorphMin, MorphMax);
    }

    float VelocityShaper::MorphMin() const {
        return _LUT.MorphMin();
    }

    float VelocityShaper::MorphMax() const {
        return _LUT.MorphMax();
    }

    size_t VelocityShaper::GetVelocityDepth() const {
        return _LUT.Width();
    }

    size_t VelocityShaper::GetMorphDepth() const {
        return _LUT.Height();
    }

    void VelocityShaper::SetGain(float Gain) {
        float min = _LUT.GainMin();
        float max = _LUT.GainMax();
        size_t depth = GetGainDepth();

        if (Gain < min) Gain = min;
        else if (Gain > max) Gain = max;

        _CurrentGain = Gain;
    }

    float VelocityShaper::Gain() const {
        return _CurrentGain;
    }

    void VelocityShaper::SetGainRange(float GainMin, float GainMax) {
        _LUT.SetGainRange(GainMin, GainMax);
    }

    float VelocityShaper::GainMin() const {
        return _LUT.GainMin();
    }

    float VelocityShaper::GainMax() const {
        return _LUT.GainMax();
    }

    size_t VelocityShaper::GetGainDepth() const {
        return _LUT.Depth();
    }

    void VelocityShaper::SetVelocityRange(uint8_t minVel, uint8_t maxVel) {
        _minVel = minVel;
        _maxVel = maxVel;
    }

    void VelocityShaper::SetVelocityMin(uint8_t minVel) {
        _minVel = minVel;
    }

    void VelocityShaper::SetVelocityMax(uint8_t maxVel) {
        _maxVel = maxVel;
    }

    uint8_t VelocityShaper::MinVelocity() const {
        return _minVel;
    }

    uint8_t VelocityShaper::MaxVelocity() const {
        return _maxVel;
    }

    bool VelocityShaper::LUT_Resize(size_t X, size_t Morph, size_t Gain) {
        return _LUT.ResizeLUT(X, Morph, Gain);
    }
    

    bool VelocityShaper::LUT_ResizeVel(size_t X) {
        return _LUT.ResizeLUT(X, _LUT.Height(), _LUT.Depth());
    }
    

    bool VelocityShaper::LUT_ResizeMorph(size_t Morph) {
        return _LUT.ResizeLUT(_LUT.Width(), Morph, _LUT.Depth());
    }
    

    bool VelocityShaper::LUT_ResizeGain(size_t Gain) {
        return _LUT.ResizeLUT(_LUT.Width(), _LUT.Height(), Gain);
    }
    
    const MIDILAR::dspFoundation::LUT::Shaping::GammaLUT<uint8_t>& VelocityShaper::GetLUT() const {
        return _LUT; 
    }

} // namespace MIDILAR::MidiDeviceBases
