#ifndef MIDILAR_DSPFOUNDATION_GENERATORS_ENVELOPES_LINEARSEGMENT_H
#define MIDILAR_DSPFOUNDATION_GENERATORS_ENVELOPES_LINEARSEGMENT_H

#include <MIDILAR_BuildSettings.h>
#include <stddef.h>

namespace MIDILAR::dspFoundation::Generators::Envelopes {

    template<typename OUTPUT_TYPE>
    void LinearSegment(
        OUTPUT_TYPE* Buffer,
        OUTPUT_TYPE StartValue,
        OUTPUT_TYPE EndValue,
        size_t TotalSamples,
        size_t FirstSample = 0,
        size_t LastSample = static_cast<size_t>(-1)
    ) {
        // Validate parameters
        if (!Buffer) return;
        if (TotalSamples == 0) return;
        if (FirstSample >= TotalSamples) return;

        // Default LastSample to the full buffer length
        if (LastSample == static_cast<size_t>(-1) || LastSample > TotalSamples) {
            LastSample = TotalSamples;
        }

        // Validate range
        if (FirstSample >= LastSample) return;

        size_t LocalLength = LastSample - FirstSample;

        // Single sample segment
        if (LocalLength == 1) {
            Buffer[FirstSample] = StartValue;
            return;
        }

        float Start = static_cast<float>(StartValue);
        float Delta = static_cast<float>(EndValue) - Start;
        float InvLength = 1.0f / static_cast<float>(LocalLength - 1);

        for (size_t i = 0; i < LocalLength; i++) {
            float t = static_cast<float>(i) * InvLength;
            float Value = Start + (Delta * t);
            Buffer[FirstSample + i] = static_cast<OUTPUT_TYPE>(Value);
        }
    }

}

#endif // MIDILAR_DSPFOUNDATION_GENERATORS_ENVELOPES_LINEARSEGMENT_H