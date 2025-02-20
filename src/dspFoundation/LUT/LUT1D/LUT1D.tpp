#ifndef MIDILAR_LOOK_UP_TABLE_TPP
#define MIDILAR_LOOK_UP_TABLE_TPP

#include "LUT1D.h"

namespace{
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
    template<typename OUTPUT_TYPE>
    bool LUT1D<OUTPUT_TYPE>::_ResizeBuffer(size_t newSize) {
        if (newSize == 0){
            if(_buffer){ free(_buffer); }
            _bufferSize = 0;    
            return true;
        }
        OUTPUT_TYPE* newBuffer = static_cast<OUTPUT_TYPE*>(malloc(newSize * sizeof(OUTPUT_TYPE)));
        
        if (!newBuffer){ return false; }

        if (_buffer) { free(_buffer); }

        _buffer = newBuffer;
        _bufferSize = newSize;
        return true;
    }
    
    template<typename OUTPUT_TYPE>
    bool LUT1D<OUTPUT_TYPE>::SetBufferSize(size_t size) {
        if (size == _bufferSize) return true;  // Skip if no change
        if (_ResizeBuffer(size)) {
            
            Eval(); // Ensure buffer is recalculated
    
            return true;
        }
        return false;
    }

    template<typename OUTPUT_TYPE>
    size_t LUT1D<OUTPUT_TYPE>::BufferSize() const{
        return _bufferSize;
    }

    template<typename OUTPUT_TYPE>
    void LUT1D<OUTPUT_TYPE>::SetData(size_t index, OUTPUT_TYPE data){
        if(index < _bufferSize){
            _buffer[index] = data;
        }
    }

    template<typename OUTPUT_TYPE>
    void LUT1D<OUTPUT_TYPE>::ClearBuffer() {
        for(size_t i = 0; i < _bufferSize; i++){
            _buffer[i] = OUTPUT_TYPE();
        }
    }

    template<typename OUTPUT_TYPE>
    LUT1D<OUTPUT_TYPE>::LUT1D(LUT1D&& other) noexcept 
        : _bufferSize(other._bufferSize), _buffer(other._buffer) // Transfer ownership
    {
        other._buffer = nullptr; // Prevent double free
        other._bufferSize = 0; // Ensure moved-from object is in a valid state
    }

    template<typename OUTPUT_TYPE>
    LUT1D<OUTPUT_TYPE>& LUT1D<OUTPUT_TYPE>::operator=(LUT1D&& other) noexcept {
        if (this != &other) { // Self-assignment check
            free(_buffer); // Free existing buffer before moving

            _buffer = other._buffer; // Move buffer pointer
            _bufferSize = other._bufferSize; // Move buffer size

            other._buffer = nullptr; // Prevent double free
            other._bufferSize = 0; // Ensure moved-from object is in a valid state
        }
        return *this;
    }

    template<typename OUTPUT_TYPE>
    void LUT1D<OUTPUT_TYPE>::SetRawData(OUTPUT_TYPE* Data, size_t buffer_size) {
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

    template<typename OUTPUT_TYPE>
    LUT1D<OUTPUT_TYPE>::LUT1D()
        : _buffer(nullptr), _bufferSize(0) {}

    template<typename OUTPUT_TYPE>
    LUT1D<OUTPUT_TYPE>::~LUT1D() {
        if (_buffer) {
            free(_buffer);
        }
    }

    template<typename OUTPUT_TYPE>
    OUTPUT_TYPE LUT1D<OUTPUT_TYPE>::GetValue(size_t inputValue) const {
        if (!_buffer || _bufferSize == 0) {
            return OUTPUT_TYPE(); // Return default value if buffer is uninitialized
        }
        
        size_t AdjustedIn = (inputValue<_bufferSize) ? inputValue : _bufferSize-1;
    
        return _buffer[AdjustedIn];
    }
    
} // namespace MIDILAR::dspFoundation::LUT

#endif // MIDILAR_LOOK_UP_TABLE_TPP
