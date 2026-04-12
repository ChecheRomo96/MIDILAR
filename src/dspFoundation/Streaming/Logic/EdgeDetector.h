#ifndef MIDILAR_DSPFOUNDATION_STREAMING_EDGEDETECTOR_H
#define MIDILAR_DSPFOUNDATION_STREAMING_EDGEDETECTOR_H

#include <System/BuildSettings.h>
#include <stddef.h>
#include <stdint.h>
#include <dspFoundation/Streaming/Logic/LevelTransitionTracker.h>

namespace MIDILAR::dspFoundation::Streaming::Logic {

    /**
     * @brief EdgeDetector detects rising/falling edges and held states in boolean input signals.
     * 
     * It outputs an EdgeState per input sample and retains state between calls.
     */
    class EdgeDetector : public LevelTransitionTracker<bool> {

    private:
        using LevelTransitionTracker<bool>::SetThreshold;
        using LevelTransitionTracker<bool>::SetThresholds;
    
    public:
    
        explicit EdgeDetector(bool initialState = false) {
            this->SetThresholds(0, 1);
            _lastStatus = initialState;
        }
    
        void ProcessBuffer(const bool* input, ThresholdState* output, size_t size) override {
            for (size_t i = 0; i < size; ++i) {
                if (!_lastStatus && input[i]) {
                    _lastStatus = true;
                    output[i] = ThresholdState::Rising;
                } else if (_lastStatus && !input[i]) {
                    _lastStatus = false;
                    output[i] = ThresholdState::Falling;
                } else if (_lastStatus) {
                    output[i] = ThresholdState::IdleHigh;
                } else {
                    output[i] = ThresholdState::IdleLow;
                }
            }
        }
    
        void Reset() override {
            _lastStatus = false;
        }
    };
        

} // namespace MIDILAR::dspFoundation::Streaming

#endif // MIDILAR_DSPFOUNDATION_STREAMING_EDGEDETECTOR_H
