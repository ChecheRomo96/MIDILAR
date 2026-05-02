#ifndef MIDILAR_DSP_INTERPOLATION_TRILINEAR_H
#define MIDILAR_DSP_INTERPOLATION_TRILINEAR_H

#include <MIDILAR_BuildSettings.h>

namespace MIDILAR::DspCore::Interpolation {

    /**
    * @brief Performs trilinear interpolation on a 2x2x2 grid.
    * 
    * @param x0 Lower bound along X axis
    * @param x1 Upper bound along X axis
    * @param y0 Lower bound along Y axis
    * @param y1 Upper bound along Y axis
    * @param z0 Lower bound along Z axis
    * @param z1 Upper bound along Z axis
    * @param f000 Value at (0,0,0)
    * @param f001 Value at (0,0,1)
    * @param f010 Value at (0,1,0)
    * @param f011 Value at (0,1,1)
    * @param f100 Value at (1,0,0)
    * @param f101 Value at (1,0,1)
    * @param f110 Value at (1,1,0)
    * @param f111 Value at (1,1,1)
    * @param x Query coordinate along X axis
    * @param y Query coordinate along Y axis
    * @param z Query coordinate along Z axis
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

} // namespace MIDILAR::DspCore::Interpolation

#endif // MIDILAR_DSP_INTERPOLATION_TRILINEAR_H
