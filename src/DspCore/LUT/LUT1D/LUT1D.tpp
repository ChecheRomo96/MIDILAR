namespace MIDILAR::DspCore::LUT {

//=============================================================
// Private
//=============================================================

template<typename T>
bool LUT1D<T>::_Reallocate(size_t newCapacity) {
    if (newCapacity == _capacity) {
        return true;
    }

    if (newCapacity == 0) {
        if (_buffer) {
            free(_buffer);
        }
        _buffer = nullptr;
        _capacity = 0;
        _size = 0;
        return true;
    }

    T* newBuffer = static_cast<T*>(malloc(newCapacity * sizeof(T)));
    if (!newBuffer) {
        return false;
    }

    // Copy existing data (up to new capacity)
    size_t copySize = (_size < newCapacity) ? _size : newCapacity;
    for (size_t i = 0; i < copySize; ++i) {
        newBuffer[i] = _buffer[i];
    }

    if (_buffer) {
        free(_buffer);
    }

    _buffer = newBuffer;
    _capacity = newCapacity;

    if (_size > _capacity) {
        _size = _capacity;
    }

    return true;
}

//=============================================================
// Constructors / Destructor
//=============================================================

template<typename T>
LUT1D<T>::LUT1D()
    : _buffer(nullptr), _size(0), _capacity(0) {}

template<typename T>
LUT1D<T>::LUT1D(size_t size)
    : _buffer(nullptr), _size(0), _capacity(0) {
    Resize(size);
}

template<typename T>
LUT1D<T>::LUT1D(LUT1D&& other) noexcept
    : _buffer(other._buffer),
      _size(other._size),
      _capacity(other._capacity) {
    other._buffer = nullptr;
    other._size = 0;
    other._capacity = 0;
}

template<typename T>
LUT1D<T>& LUT1D<T>::operator=(LUT1D&& other) noexcept {
    if (this != &other) {
        if (_buffer) {
            free(_buffer);
        }

        _buffer = other._buffer;
        _size = other._size;
        _capacity = other._capacity;

        other._buffer = nullptr;
        other._size = 0;
        other._capacity = 0;
    }
    return *this;
}

template<typename T>
LUT1D<T>::~LUT1D() noexcept {
    if (_buffer) {
        free(_buffer);
    }
}

//=============================================================
// Memory management
//=============================================================

template<typename T>
bool LUT1D<T>::Reserve(size_t newCapacity) {
    if (newCapacity <= _capacity) {
        return true;
    }
    return _Reallocate(newCapacity);
}

template<typename T>
bool LUT1D<T>::Resize(size_t newSize) {
    if (newSize > _capacity) {
        if (!_Reallocate(newSize)) {
            return false;
        }
    }

    _size = newSize;
    return true;
}

template<typename T>
bool LUT1D<T>::SetBufferSize(size_t size) {
    return Resize(size);
}

//=============================================================
// Accessors
//=============================================================

template<typename T>
size_t LUT1D<T>::Size() const {
    return _size;
}

template<typename T>
size_t LUT1D<T>::BufferSize() const {
    return _size;
}

template<typename T>
size_t LUT1D<T>::Capacity() const {
    return _capacity;
}

template<typename T>
T* LUT1D<T>::GetBuffer() {
    return _buffer;
}

template<typename T>
const T* LUT1D<T>::GetBuffer() const {
    return _buffer;
}

//=============================================================
// Data access
//=============================================================

template<typename T>
T LUT1D<T>::GetValue(size_t bin) const {
    if (bin >= _size || !_buffer) {
        return T{};
    }
    return _buffer[bin];
}

template<typename T>
void LUT1D<T>::SetBinValue(size_t index, T value) {
    if (index >= _size || !_buffer) {
        return;
    }
    _buffer[index] = value;
}

template<typename T>
void LUT1D<T>::SetData(size_t index, T value) {
    SetBinValue(index, value);
}

//=============================================================
// Utilities
//=============================================================

template<typename T>
void LUT1D<T>::Clear() {
    if (!_buffer) {
        return;
    }

    for (size_t i = 0; i < _size; ++i) {
        _buffer[i] = T{};
    }
}

template<typename T>
void LUT1D<T>::ClearBuffer() {
    Clear();
}

template<typename T>
bool LUT1D<T>::SetRawData(const T* data, size_t bufferSize) {
    if (!data && bufferSize > 0) {
        return false;
    }

    if (!Resize(bufferSize)) {
        return false;
    }

    for (size_t i = 0; i < _size; ++i) {
        _buffer[i] = data[i];
    }

    return true;
}

} // namespace MIDILAR::DspCore::LUT