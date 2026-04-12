#ifndef MIDILAR_DSPFOUNDATION_STREAMING_LOGIC_STATECHANGe_H
#define MIDILAR_DSPFOUNDATION_STREAMING_LOGIC_STATECHANGe_H

#include <System/BuildSettings.h>
#include <dspFoundation/Streaming/StreamingBase.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

namespace MIDILAR::dspFoundation::Streaming::Logic {

/**
 * @brief Emits a mapped output only when the input changes from the previous value.
 * 
 * The mapping is defined via SetMap(), where each entry maps an input value to an output value.
 * 
 * If an input is not present in the mapping table, the gate outputs the input unchanged on state change.
 * For repeated inputs, the gate emits zero.
 */
template<typename TIn, typename TOut>
class StateChange : public StreamingBase<TIn, TOut> {
private:
    TIn* _mapIn = nullptr;
    TOut* _mapOut = nullptr;
    size_t _mapSize = 0;

    TIn _lastValue{};
    bool _first = true;

public:
    ~StateChange() {
        if (_mapIn)  free(_mapIn);
        if (_mapOut) free(_mapOut);
    }

    /**
     * @brief Set the mapping of input → output.
     * Both arrays must contain `count` entries.
     * 
     * For inputs found in the `mapIn` array, the corresponding output from `mapOut` is emitted
     * on state change. If an input is not found in the `mapIn` array, its value is passed through
     * unchanged on change detection (i.e., output = input).
     */
    void SetMap(const TIn* mapIn, const TOut* mapOut, size_t count) {
        if (_mapIn)  free(_mapIn);
        if (_mapOut) free(_mapOut);

        _mapIn = static_cast<TIn*>(malloc(sizeof(TIn) * count));
        _mapOut = static_cast<TOut*>(malloc(sizeof(TOut) * count));
        _mapSize = count;

        for (size_t i = 0; i < count; ++i) {
            _mapIn[i] = mapIn[i];
            _mapOut[i] = mapOut[i];
        }
    }

    /**
     * @brief Process a buffer of input values. Outputs the mapped value only if the input changes;
     * otherwise, outputs zero.
     */
    void ProcessBuffer(const TIn* input, TOut* output, size_t size) override {
        for (size_t i = 0; i < size; ++i) {
            TIn current = input[i];

            if (_first || current != _lastValue) {
                _lastValue = current;
                _first = false;

                // Search for a mapped output
                bool found = false;
                for (size_t j = 0; j < _mapSize; ++j) {
                    if (_mapIn[j] == current) {
                        output[i] = _mapOut[j];
                        found = true;
                        break;
                    }
                }

                // Fallback: output = input if not mapped
                output[i] = found ? output[i] : static_cast<TOut>(current);
            } else {
                output[i] = static_cast<TOut>(0);
            }
        }
    }

    /**
     * @brief Reset the internal state to detect the next change.
     */
    void Reset() override {
        _first = true;
        _lastValue = TIn{};
    }
};

} // namespace MIDILAR::dspFoundation::Streaming::Logic

#endif // MIDILAR_DSPFOUNDATION_STREAMING_LOGIC_STATECHANGe_H
