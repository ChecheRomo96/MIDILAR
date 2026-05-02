#ifndef MIDILAR_DSP_STREAMING_H
#define MIDILAR_DSP_STREAMING_H

    #include <MIDILAR_BuildSettings.h>

    namespace MIDILAR::DspCore::Streaming{

    template<typename TIn, typename TOut>
    class StreamingBase {
    public:
                
        /**
         * @brief Process a single sample and return the result.
         */
        virtual TOut ProcessSample(const TIn& input) {
            TOut tmp{};
            ProcessBuffer(&input, &tmp, 1);
            return tmp;
        }

        /**
         * @brief Process a single sample and write the result into an existing buffer.
         */
        virtual void ProcessSample(const TIn& input, TOut* output) {
            ProcessBuffer(&input, output, 1);
        }

        virtual void ProcessBuffer(const TIn* input, TOut* output, size_t size) = 0; 

        virtual void Reset() = 0;
        virtual bool IsActive() const { return true; }

        virtual ~StreamingBase() = default;
    };

    }

#endif//MIDILAR_DSP_STREAMING_H
