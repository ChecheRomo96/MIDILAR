#ifndef MIDILAR_LEVEL_TRANSITION_TRACKER_H
#define MIDILAR_LEVEL_TRANSITION_TRACKER_H

    #include <System/BuildSettings.h>
    #include <dspFoundation/Streaming/StreamingBase.h>
    #include <stdint.h>

    namespace MIDILAR::dspFoundation::Streaming::Logic{

        enum class ThresholdState : uint8_t {
            IdleLow = 0,
            Rising = 1,
            IdleHigh = 2,
            Falling = 3
        };

        template<typename T>
        class LevelTransitionTracker : public StreamingBase<T, ThresholdState> {
        protected:
            T _lowerThreshold;
            T _upperThreshold;
            bool _lastStatus = false;

        public:
            LevelTransitionTracker(T lower = T(0.5), T upper = T(0.5)) 
                : _lowerThreshold(lower), _upperThreshold(upper) {}

            
            void SetThresholds(T lower, T upper) {
                _lowerThreshold = lower;
                _upperThreshold = upper;
            }

            /**
             * @brief Sets both the upper and lower thresholds to the same value.
             * @param value The threshold value for rising/falling edge detection.
             */
            void SetThreshold(T value) {
                _lowerThreshold = value;
                _upperThreshold = value;
            }

            virtual void ProcessBuffer(const T* input, ThresholdState* output, size_t size) override = 0;

            virtual void Reset() override {
                // intentionally empty
            }

        };
    }
    
#endif//MIDILAR_LEVEL_TRANSITION_TRACKER_H