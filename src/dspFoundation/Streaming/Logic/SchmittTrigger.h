#ifndef MIDILAR_DSPFOUNDATION_STREAMING_SCHMITTTRIGGER_H
#define MIDILAR_DSPFOUNDATION_STREAMING_SCHMITTTRIGGER_H

#include <System/BuildSettings.h>
#include <stddef.h>
#include <stdint.h>
#include <dspFoundation/Streaming/Logic/LevelTransitionTracker.h>

namespace MIDILAR::dspFoundation::Streaming::Logic {

    /**
     * @brief SchmittTrigger implements a thresholded boolean switch with hysteresis.
     * 
     * The output is true when input rises above the upper threshold,
     * and remains true until the input falls below the lower threshold.
     * @tparam T Input Type
     */
    template<typename T>
    class SchmittTrigger : public LevelTransitionTracker<T> {

    private:
        using LevelTransitionTracker<T>::_lastStatus;
    
    public:
        /**
         * @brief Construct a SchmittTrigger with optional thresholds.
         * @param lower Lower threshold for turning off (default = 0.5)
         * @param upper Upper threshold for turning on (default = 0.5)
         */
        explicit SchmittTrigger(T lower = 0.3f, T upper = 0.7f)
            : LevelTransitionTracker<T>(lower, upper) {}

        /**
         * @brief Process a buffer of input samples and emit boolean output.
         * @param input Pointer to input array
         * @param output Pointer to output boolean array
         * @param size Number of samples
         */
        void ProcessBuffer(const T* input, bool* output, size_t size) override {
            for (size_t i = 0; i < size; ++i) {
                if (!this->_lastStatus && input[i] >= this->_upperThreshold) {
                    this->_lastStatus = true;
                } else if (this->_lastStatus && input[i] <= this->_lowerThreshold) {
                    this->_lastStatus = false;
                }
                output[i] = this->_lastStatus;
            }
        }

        /**
         * @brief Reset the output state to LOW (false).
         */
        void Reset() override {
            _lastStatus = false;
        }
    };

} // namespace MIDILAR::dspFoundation::Streaming::Logic

#endif // MIDILAR_DSPFOUNDATION_STREAMING_SCHMITTTRIGGER_H
