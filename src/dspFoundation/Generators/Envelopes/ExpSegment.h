#ifndef MIDILAR_DSPFOUNDATION_GENERATORS_ENVELOPES_EXPSEGMENT_H
#define MIDILAR_DSPFOUNDATION_GENERATORS_ENVELOPES_EXPSEGMENT_H

#include <MIDILAR_BuildSettings.h>
#include <stddef.h>
#include <math.h>

namespace MIDILAR::dspFoundation::Generators::Envelopes {

    template<typename OUTPUT_TYPE>
    void ExpSegment(
        OUTPUT_TYPE* Buffer,
        OUTPUT_TYPE StartValue,
        OUTPUT_TYPE EndValue,
        float Curve,
        size_t TotalSamples,
        size_t FirstSample = 0,
        size_t LastSample = static_cast<size_t>(-1)
    ) {
        // Validate parameters
        if (!Buffer) return;
        if (TotalSamples == 0) return;
        if (FirstSample >= TotalSamples) return;

        // Clamp curve to avoid invalid exponentials
        if (Curve <= 0.0f) {
            Curve = 0.0001f;
        }

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

        // Normalize so that t = 1 maps exactly to 1
        float Denominator = 1.0f - expf(-Curve);

        for (size_t i = 0; i < LocalLength; i++) {
            float t = static_cast<float>(i) * InvLength;

            // Fast at the beginning, slower near the end
            float Shaped = (1.0f - expf(-Curve * t)) / Denominator;

            float Value = Start + (Delta * Shaped);

            Buffer[FirstSample + i] = static_cast<OUTPUT_TYPE>(Value);
        }
    }

}

#endif // MIDILAR_DSPFOUNDATION_GENERATORS_ENVELOPES_EXPSEGMENT_H