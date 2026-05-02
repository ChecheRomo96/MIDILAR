#ifndef MIDILAR_SYSTEM_RINGBUFFER_H
#define MIDILAR_SYSTEM_RINGBUFFER_H

#include <stddef.h>

namespace MIDILAR::SystemCore {

    template <typename T>
    class RingBuffer {
    private:
        T* _buffer;
        size_t _size;

        size_t _readIndex;
        size_t _writeIndex;
        size_t _available;

    public:
        RingBuffer(T* buffer, size_t size);

        bool Push(const T& value);
        bool Pop(T& out);

        size_t GetAvailable() const;
        size_t GetFreeSpace() const;

        bool IsEmpty() const;
        bool IsFull() const;

        void Reset();
    };

}

#include "RingBuffer.tpp"

#endif//MIDILAR_SYSTEM_RINGBUFFER_H
