#ifndef MIDILAR_LOOK_UP_TABLE_TPP
#define MIDILAR_LOOK_UP_TABLE_TPP

#include "LUT1D.h"

namespace{
    template<typename T>
    struct is_floating_point {
        static constexpr bool value = false;  // Default: Not a floating-point type
    };

    // Specializations for float, double, and long double
    template<>
    struct is_floating_point<float> {
        static constexpr bool value = true;
    };

    template<>
    struct is_floating_point<double> {
        static constexpr bool value = true;
    };

    template<>
    struct is_floating_point<long double> {
        static constexpr bool value = true;
    };
    template<typename T>
    inline constexpr T min_value(T a, T b) {
        return (a < b) ? a : b;
    }

    template<typename T>
    inline constexpr T max_value(T a, T b) {
        return (a > b) ? a : b;
    }
}

namespace MIDILAR::dspFoundation::LUT {
    
    template<typename INPUT_TYPE, typename OUTPUT_TYPE>
    void LUT1D<INPUT_TYPE, OUTPUT_TYPE>::DetectMode() {
        _Pipeline = Bypass;  // Default: No processing

        // FLOATING-POINT CASE
        if constexpr (is_floating_point<INPUT_TYPE>::value) {
            _Pipeline = CombineFlags(_Pipeline, Interpolation);  // Floating-Point Always Interpolates

            if (static_cast<size_t>(abs(_InputRangeMax - _InputRangeMin)) != _bufferSize) {
                _Pipeline = CombineFlags(_Pipeline, Scaling);  // Scaling Needed if LUT Size != Input Range
            }
            if (_InputRangeMin != 0.0 || _InputRangeMax != _bufferSize - 1.0) {
                _Pipeline = CombineFlags(_Pipeline, Offsetting);  // Offset Needed if Input Min/Max is Misaligned
            }
        }
        // INTEGER CASE
        else {
            size_t inputRange = _InputRangeMax - _InputRangeMin;
            if (_bufferSize < inputRange) {
                _Pipeline = CombineFlags(_Pipeline, Scaling);
                _Pipeline = CombineFlags(_Pipeline, Interpolation);  // Scaling + Interpolation Needed
            } else if (_bufferSize > inputRange) {
                _Pipeline = CombineFlags(_Pipeline, Scaling);
            }

            if (_InputRangeMin != 0 || _InputRangeMax != _bufferSize - 1) {
                _Pipeline = CombineFlags(_Pipeline, Offsetting);  // Offset Needed if Input Min/Max is Misaligned
            }
        }
    }

    
    template<typename INPUT_TYPE, typename OUTPUT_TYPE>
    void LUT1D<INPUT_TYPE, OUTPUT_TYPE>::_RecalculateConstants(){
        
        // Avoid division by zero and handle degenerate cases
        if (_bufferSize > 1 && (_InputRangeMax - _InputRangeMin) > 1e-6f) {
            _InputGain = (_bufferSize) / (_InputRangeMax - _InputRangeMin);
            _InputOffset = -_InputRangeMin * _InputGain; // Precomputed offset
        } else {
            _InputGain = 1.0f;
            _InputOffset = 0.0f;
        }

    }

    template<typename INPUT_TYPE, typename OUTPUT_TYPE>
    bool LUT1D<INPUT_TYPE, OUTPUT_TYPE>::_ResizeBuffer(size_t newSize) {
        if (newSize == 0) return false; // Prevent zero allocation

        OUTPUT_TYPE* newBuffer = static_cast<OUTPUT_TYPE*>(malloc(newSize * sizeof(OUTPUT_TYPE)));
        if (!newBuffer){
            return false;
        }

        if (_buffer) {
            free(_buffer);
        }

        _buffer = newBuffer;
        _bufferSize = newSize;
        return true;
    }
    
    template<typename INPUT_TYPE, typename OUTPUT_TYPE>
    bool LUT1D<INPUT_TYPE, OUTPUT_TYPE>::SetBufferSize(size_t size) {
        if (size == _bufferSize) return true;  // Skip if no change
        if (_ResizeBuffer(size)) {
            
            DetectMode();
            _RecalculateConstants();
            Eval(); // Ensure buffer is recalculated
    
            return true;
        }
        return false;
    }

    template<typename INPUT_TYPE, typename OUTPUT_TYPE>
    size_t LUT1D<INPUT_TYPE, OUTPUT_TYPE>::BufferSize() const{
        return _bufferSize;
    }

    template<typename INPUT_TYPE, typename OUTPUT_TYPE>
    void LUT1D<INPUT_TYPE, OUTPUT_TYPE>::ClearBuffer() {
        if (_buffer) {
            for (size_t i = 0; i < _bufferSize; i++) {
                _buffer[i] = OUTPUT_TYPE();
            }
        }
    }

    template<typename INPUT_TYPE, typename OUTPUT_TYPE>
    LUT1D<INPUT_TYPE, OUTPUT_TYPE>::LUT1D()
        : _buffer(nullptr), _bufferSize(0), _InputRangeMin(0.0f), _InputRangeMax(1.0f) {}

    template<typename INPUT_TYPE, typename OUTPUT_TYPE>
    LUT1D<INPUT_TYPE, OUTPUT_TYPE>::~LUT1D() {
        if (_buffer) {
            free(_buffer);
        }
    }

    template<typename INPUT_TYPE, typename OUTPUT_TYPE>
    void LUT1D<INPUT_TYPE, OUTPUT_TYPE>::SetInputRange(INPUT_TYPE minVal, INPUT_TYPE maxVal) {
        if (minVal == _InputRangeMin && maxVal == _InputRangeMax) return;  // Skip if unchanged

        _InputRangeMin = minVal;
        _InputRangeMax = maxVal;

        DetectMode();
        _RecalculateConstants();
        Eval(); // Ensure buffer is recalculated
    }

    template<typename INPUT_TYPE, typename OUTPUT_TYPE>
    inline INPUT_TYPE LUT1D<INPUT_TYPE, OUTPUT_TYPE>::InputMin() const {
        return _InputRangeMin;
    }

    template<typename INPUT_TYPE, typename OUTPUT_TYPE>
    inline INPUT_TYPE LUT1D<INPUT_TYPE, OUTPUT_TYPE>::InputMax() const {
        return _InputRangeMax;
    }

    template<typename INPUT_TYPE, typename OUTPUT_TYPE>
    OUTPUT_TYPE LUT1D<INPUT_TYPE, OUTPUT_TYPE>::GetValue(INPUT_TYPE inputValue, bool Interpolate, bool Wrapping) const {
        if (!_buffer || _bufferSize == 0) {
            return OUTPUT_TYPE(); // Return default value if buffer is uninitialized
        }
    
        // Handle Wrapping Mode (Cyclic Behavior)
        if (Wrapping) {
            float tmp = fmodf(inputValue - _InputRangeMin, _InputRangeMax - _InputRangeMin);
            if (tmp < 0) {
                tmp += (_InputRangeMax - _InputRangeMin);
            }
            inputValue = _InputRangeMin + tmp;
        } 
        // Default: Clamp Input Before Any Processing
        else {
            inputValue = max_value(_InputRangeMin, min_value(inputValue, _InputRangeMax));
        }

        // If No Processing is Needed, Return Direct Output
        if (HasPipelineFlag(InputPipeline::Bypass)) {
            return _buffer[static_cast<size_t>(inputValue)];
        }
    
        // Convert Input to Float for Processing
        float inputProcessed = static_cast<float>(inputValue);
    
        // Apply Scaling
        if (HasPipelineFlag(InputPipeline::Scaling)) {
            inputProcessed *= _InputGain;
        }
    
        // Apply Offsetting
        if (HasPipelineFlag(InputPipeline::Offsetting)) {
            inputProcessed += _InputOffset;
        }
    
        // Clamp to Valid LUT Index Range
        inputProcessed = max_value(0.0f, min_value(static_cast<float>(_bufferSize), inputProcessed));
    
        // Determine LUT Indices
        size_t lowerIndex = static_cast<size_t>(inputProcessed);
        size_t upperIndex = (lowerIndex + 1) % _bufferSize;  // Wrap-around if needed
    
        // No Interpolation? Return Direct LUT Value
        if (!HasPipelineFlag(InputPipeline::Interpolation) || !Interpolate) {
            return _buffer[lowerIndex];
        }
    
        // Compute Interpolation Factor
        float interpolationFactor = inputProcessed - lowerIndex;
    
        // Perform Linear Interpolation
        OUTPUT_TYPE lowerValue = _buffer[lowerIndex];
        OUTPUT_TYPE upperValue = _buffer[upperIndex];
    
        return static_cast<OUTPUT_TYPE>(lowerValue + (upperValue - lowerValue) * interpolationFactor);
    }
    

    template<typename INPUT_TYPE, typename OUTPUT_TYPE>
    void LUT1D<INPUT_TYPE, OUTPUT_TYPE>::SetRawData(OUTPUT_TYPE* Data, size_t buffer_size) {
        if (Data == nullptr || buffer_size == 0) return;  // Protect against null pointer or zero size
        
        if(BufferSize() != buffer_size){
            if(!_ResizeBuffer(buffer_size)){
                // Allocation Error
                return;
            }
        }

        for(size_t i = 0; i < buffer_size; i++){
            _buffer[i] = Data[i];
        }
    }

} // namespace MIDILAR::dspFoundation::LUT

#endif // MIDILAR_LOOK_UP_TABLE_TPP
