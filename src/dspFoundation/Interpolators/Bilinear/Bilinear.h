#ifndef MIDILAR_DSP_INTERPOLATION_BILINEAR_H
#define MIDILAR_DSP_INTERPOLATION_BILINEAR_H

#include <System/BuildSettings.h>

namespace MIDILAR::dspFoundation::Interpolation {

/**
 * @brief Performs bilinear interpolation on a 2x2 grid.
 * 
 * @tparam T Data type of grid values
 * @param x0 X coordinate of the left column
 * @param x1 X coordinate of the right column
 * @param y0 Y coordinate of the bottom row
 * @param y1 Y coordinate of the top row
 * @param f00 Value at (x0, y0)
 * @param f10 Value at (x1, y0)
 * @param f01 Value at (x0, y1)
 * @param f11 Value at (x1, y1)
 * @param x  Query x
 * @param y  Query y
 * @return Interpolated value at (x, y)
 */
template<typename T>
inline float Bilinear(
    float x0, float x1,
    float y0, float y1,
    T f00, T f10,
    T f01, T f11,
    float x, float y
) {
    float tx = (x - x0) / (x1 - x0);
    float ty = (y - y0) / (y1 - y0);

    float a = (1.0f - tx) * static_cast<float>(f00) + tx * static_cast<float>(f10);
    float b = (1.0f - tx) * static_cast<float>(f01) + tx * static_cast<float>(f11);
    return (1.0f - ty) * a + ty * b;
}

} // namespace MIDILAR::dspFoundation::Interpolation

#endif // MIDILAR_DSP_INTERPOLATION_BILINEAR_H
