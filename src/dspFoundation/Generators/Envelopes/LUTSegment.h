#ifndef MIDILAR_DSPFOUNDATION_GENERATORS_ENVELOPES_LUTSEGMENT_H
#define MIDILAR_DSPFOUNDATION_GENERATORS_ENVELOPES_LUTSEGMENT_H

#include <MIDILAR_BuildSettings.h>
#include <stddef.h>

#include <dspFoundation/LUT/LUT1D/LUT1D.h>
#include <dspFoundation/Interpolators/Linear/Linear.h>

namespace MIDILAR::dspFoundation::Generators::Envelopes {

    template<typename OUTPUT_TYPE, typename LUT_TYPE>
    void LUTSegment(
        OUTPUT_TYPE* Buffer,
        OUTPUT_TYPE StartValue,
        OUTPUT_TYPE EndValue,
        size_t TotalSamples,
        const MIDILAR::dspFoundation::LUT::LUT1D<LUT_TYPE>& Table,
        size_t FirstSample = 0,
        size_t LastSample = static_cast<size_t>(-1)
    ) {
        if (!Buffer) return;
        if (TotalSamples == 0) return;
        if (FirstSample >= TotalSamples) return;
        if (Table.BufferSize() == 0) return;

        if (LastSample == static_cast<size_t>(-1) || LastSample > TotalSamples) {
            LastSample = TotalSamples;
        }

        if (FirstSample >= LastSample) return;

        size_t LocalLength = LastSample - FirstSample;

        if (LocalLength == 1) {
            Buffer[FirstSample] = StartValue;
            return;
        }

        size_t LutSize = Table.BufferSize();
        if (LutSize == 1) {
            Buffer[FirstSample] = static_cast<OUTPUT_TYPE>(StartValue);
            return;
        }

        float Start = static_cast<float>(StartValue);
        float Delta = static_cast<float>(EndValue) - Start;
        float InvLength = 1.0f / static_cast<float>(LocalLength - 1);

        for (size_t i = 0; i < LocalLength; i++) {
            float t = static_cast<float>(i) * InvLength;

            float LutIndex = t * static_cast<float>(LutSize - 1);

            size_t Index0 = static_cast<size_t>(LutIndex);
            size_t Index1 = (Index0 + 1 < LutSize) ? (Index0 + 1) : Index0;

            float Alpha = LutIndex - static_cast<float>(Index0);

            float Y0 = static_cast<float>(Table.GetValue(Index0));
            float Y1 = static_cast<float>(Table.GetValue(Index1));

            float Shaped = static_cast<float>(
                MIDILAR::dspFoundation::Interpolation::Linear(Y0, Y1, Alpha)
            );

            float Value = Start + (Delta * Shaped);
            Buffer[FirstSample + i] = static_cast<OUTPUT_TYPE>(Value);
        }
    }

}

#endif // MIDILAR_DSPFOUNDATION_GENERATORS_ENVELOPES_LUTSEGMENT_H