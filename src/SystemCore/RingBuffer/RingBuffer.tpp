#include "RingBuffer.h"

namespace MIDILAR::SystemCore {

    template <typename T>
    RingBuffer<T>::RingBuffer(T* buffer, size_t size)
        : _buffer(buffer),
          _size(size),
          _readIndex(0),
          _writeIndex(0),
          _available(0) {}

    template <typename T>
    bool RingBuffer<T>::Push(const T& value) {
        if (_available >= _size) {
            return false;
        }

        _buffer[_writeIndex] = value;

        _writeIndex++;
        if (_writeIndex >= _size) {
            _writeIndex = 0;
        }

        _available++;
        return true;
    }

    template <typename T>
    bool RingBuffer<T>::Pop(T& out) {
        if (_available == 0) {
            return false;
        }

        out = _buffer[_readIndex];

        _readIndex++;
        if (_readIndex >= _size) {
            _readIndex = 0;
        }

        _available--;
        return true;
    }

    template <typename T>
    size_t RingBuffer<T>::GetAvailable() const {
        return _available;
    }

    template <typename T>
    size_t RingBuffer<T>::GetFreeSpace() const {
        return _size - _available;
    }

    template <typename T>
    bool RingBuffer<T>::IsEmpty() const {
        return _available == 0;
    }

    template <typename T>
    bool RingBuffer<T>::IsFull() const {
        return _available == _size;
    }

    template <typename T>
    void RingBuffer<T>::Reset() {
        _readIndex = 0;
        _writeIndex = 0;
        _available = 0;
    }

}