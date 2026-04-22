// GammaLUT.tpp
#include <math.h>
#include "GammaLUT.h"
#include <dspFoundation/Generators/Shaping/Shaping.h>

namespace MIDILAR::dspFoundation::LUT::Shaping {

template<typename T>
inline float GammaLUT<T>::Clamp(float value, float min, float max) const {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

template<typename T>
inline GammaLUT<T>::GammaLUT()
: _gainMin(static_cast<T>(1.0f)),
  _gainMax(static_cast<T>(10.0f)),
  _morphMin(static_cast<T>(-1.0f)),
  _morphMax(static_cast<T>(1.0f)),
  _outputGain(static_cast<T>(1.0f))
{
}

template<typename T>
inline void GammaLUT<T>::Eval() {
    const size_t xSize = this->Width();
    const size_t ySize = this->Height();
    const size_t zSize = this->Depth();

    const bool singleMorph = (ySize == 1);
    const bool singleGain  = (zSize == 1);
    
    for (size_t k = 0; k < zSize; ++k) {
        float gain = singleGain ? _gainMin : _gainMin + (_gainMax - _gainMin) * static_cast<float>(k) / (zSize - 1);
        for (size_t j = 0; j < ySize; ++j) {
            float morph = singleMorph ? _morphMin : _morphMin + (_morphMax - _morphMin) * static_cast<float>(j) / (ySize - 1);
            T* row = this->GetBuffer() + (k * ySize + j) * xSize;
            MIDILAR::dspFoundation::Generators::Shaping::EnhancedGamma<T>(row, xSize, morph, gain, _outputGain);
        }
    }
}

template<typename T>
inline bool GammaLUT<T>::ResizeLUT(size_t Input, size_t Morph, size_t Gain) {
    if (Input == 0) Input = 1;
    if (Morph == 0) Morph = 1;
    if (Gain == 0) Gain = 1;
    return this->SetBufferSize(Input, Morph, Gain);
}

template<typename T>
inline void GammaLUT<T>::ResizeInput(size_t Input) {
    if (Input == 0) Input = 1;
    this->SetBufferSize(Input, this->Height(), this->Depth());
    this->Eval();
}

template<typename T>
inline void GammaLUT<T>::ResizeMorph(size_t Morph) {
    if (Morph == 0) Morph = 1;
    this->SetBufferSize(this->Width(), Morph, this->Depth());
    this->Eval();
}

template<typename T>
inline void GammaLUT<T>::ResizeGain(size_t Gain) {
    if (Gain == 0) Gain = 1;
    this->SetBufferSize(this->Width(), this->Height(), Gain);
    this->Eval();
}

template<typename T>
inline void GammaLUT<T>::SetGainRange(float GainMin, float GainMax) {
    _gainMin = GainMin;
    _gainMax = GainMax;
    this->Eval();
}

template<typename T>
inline float GammaLUT<T>::GainMin() const {
    return _gainMin;
}

template<typename T>
inline float GammaLUT<T>::GainMax() const {
    return _gainMax;
}

template<typename T>
inline void GammaLUT<T>::SetMorphRange(float MorphMin, float MorphMax) {
    _morphMin = Clamp(MorphMin, -1.0f, 1.0f);
    _morphMax = Clamp(MorphMax, -1.0f, 1.0f);
    this->Eval();
}

template<typename T>
inline float GammaLUT<T>::MorphMin() const {
    return _morphMin;
}

template<typename T>
inline float GammaLUT<T>::MorphMax() const {
    return _morphMax;
}

template<typename T>
void GammaLUT<T>::SetOutputGain(T gain){
    _outputGain = gain;
}

template<typename T>
T GammaLUT<T>::OutputGain() const{
    return _outputGain;
}

template<typename T>
float GammaLUT<T>::GetValueFloat(size_t x, size_t y, size_t z) const{
    return this->GetValue(x,y,z) * (1.0f/_outputGain);
}

template<typename T>
float GammaLUT<T>::GetValueFloat(size_t x, size_t y) const{
    return this->GetValue(x,y) * (1.0f/_outputGain);
}

template<typename T>
float GammaLUT<T>::GetValueFloat(size_t x) const{
    return this->GetValue(x) * (1.0f/_outputGain);
}

} // namespace MIDILAR::dspFoundation::LUT::Shaping
