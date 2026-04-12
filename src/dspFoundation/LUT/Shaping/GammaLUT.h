// GammaLUT.h
#ifndef MIDILAR_GAMMAMORPH_LUT3D_H
#define MIDILAR_GAMMAMORPH_LUT3D_H

#include <dspFoundation/LUT/LUT3D/LUT3D.h>

namespace MIDILAR::dspFoundation::LUT::Shaping {

    struct InterpolationMode3D {
        bool interpX = true; // Input axis
        bool interpY = true; // Morph axis
        bool interpZ = true; // Gain axis
    };

    template<typename T>
    class GammaLUT : public LUT3D<T> {
    private:
        float _gainMin = 1.0f;
        float _gainMax = 10.0f;
        float _morphMin = -1.0f;
        float _morphMax = 1.0f;
        T _outputGain = 1.0;

        float Clamp(float value, float min, float max) const;

    public:
        using Base = LUT3D<T>;

        GammaLUT();

        void Eval() override;

        bool ResizeLUT(size_t Input, size_t Morph = 1, size_t Gain = 1);
        void ResizeInput(size_t Input);
        void ResizeMorph(size_t Morph);
        void ResizeGain(size_t Gain);

        void SetGainRange(float GainMin, float GainMax = 0.0f);
        float GainMin() const;
        float GainMax() const;

        void SetMorphRange(float MorphMin, float MorphMax);
        float MorphMin() const;
        float MorphMax() const;

        void SetOutputGain(T gain);
        T OutputGain() const;

        float GetValueFloat(size_t x, size_t y, size_t z) const;
        float GetValueFloat(size_t x, size_t y) const;
        float GetValueFloat(size_t x) const;
    };

} // namespace MIDILAR::dspFoundation::LUT::Shaping

    #include "GammaLUT.tpp"

#endif // MIDILAR_GAMMAMORPH_LUT3D_H