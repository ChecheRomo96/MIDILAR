#ifndef MIDILAR_DSP_INTERPOLATION_TRILINEAR_H
#define MIDILAR_DSP_INTERPOLATION_TRILINEAR_H

#include <MIDILAR_BuildSettings.h>

namespace MIDILAR::dspFoundation::Interpolation {

    /**
    * @brief Performs trilinear interpolation on a 2x2x2 grid.
    * 
    * @tparam T Data type of grid values
    * @param x0, x1 Bounds along X axis
    * @param y0, y1 Bounds along Y axis
    * @param z0, z1 Bounds along Z axis
    * @param f000 - f111 Values at the corners of the cube
    * @param x, y, z Query coordinates
    * @return Interpolated value at (x, y, z)
    */
    template<typename T>
    inline float Trilinear(
        float x0, float x1,
        float y0, float y1,
        float z0, float z1,
        T f000, T f100, T f010, T f110,
        T f001, T f101, T f011, T f111,
        float x, float y, float z
    ) {
        float tx = (x - x0) / (x1 - x0);
        float ty = (y - y0) / (y1 - y0);
        float tz = (z - z0) / (z1 - z0);

        float c00 = (1 - tx) * static_cast<float>(f000) + tx * static_cast<float>(f100);
        float c10 = (1 - tx) * static_cast<float>(f010) + tx * static_cast<float>(f110);
        float c01 = (1 - tx) * static_cast<float>(f001) + tx * static_cast<float>(f101);
        float c11 = (1 - tx) * static_cast<float>(f011) + tx * static_cast<float>(f111);

        float c0 = (1 - ty) * c00 + ty * c10;
        float c1 = (1 - ty) * c01 + ty * c11;

        return (1 - tz) * c0 + tz * c1;
    }

} // namespace MIDILAR::dspFoundation::Interpolation

#endif // MIDILAR_DSP_INTERPOLATION_TRILINEAR_H
