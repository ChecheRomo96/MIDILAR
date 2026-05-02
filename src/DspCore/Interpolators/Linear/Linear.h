#ifndef MIDILAR_DSP_INTERPOLATION_LINEAR_H
#define MIDILAR_DSP_INTERPOLATION_LINEAR_H

#include <MIDILAR_BuildSettings.h>

namespace MIDILAR::DspCore::Interpolation {

    /**
     * @brief Linear interpolation between two points.
     *
     * @tparam T Data type of y values
     * @param x0 Position of first point
     * @param x1 Position of second point
     * @param y0 Value at x0
     * @param y1 Value at x1
     * @param x  Target x
     * @return Interpolated value at x
     */
    template<typename T>
    inline float Linear(float x0, float x1, T y0, T y1, float x) {
        if (x1 == x0) {
            return static_cast<float>(y0);
        }

        float t = (x - x0) / (x1 - x0);

        return (1.0f - t) * static_cast<float>(y0)
             + t          * static_cast<float>(y1);
    }

    /**
     * @brief Linear interpolation using a normalized parameter.
     *
     * @tparam T Data type of y values
     * @param y0 First value
     * @param y1 Second value
     * @param t  Interpolation factor in range [0,1]
     * @return Interpolated value
     */
    template<typename T>
    inline float Linear(T y0, T y1, float t) {
        return (1.0f - t) * static_cast<float>(y0)
             + t          * static_cast<float>(y1);
    }

} // namespace MIDILAR::DspCore::Interpolation

#endif // MIDILAR_DSP_INTERPOLATION_LINEAR_H