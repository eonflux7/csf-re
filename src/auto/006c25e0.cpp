#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static int __cdecl GfxAnimCompress_KeyframeToMatrix(int matrixAddress, int keyframeAddress)
{
    void* matrix = reinterpret_cast<void*>(matrixAddress);
    const void* keyframe = reinterpret_cast<const void*>(keyframeAddress);

    const float x = CSF_AT(float, keyframe, 0x08);
    const float z = CSF_AT(float, keyframe, 0x10);
    const float y = CSF_AT(float, keyframe, 0x0C);
    const float w = CSF_AT(float, keyframe, 0x14);

    const float xx = x * x;
    const float yy = y * y;
    const float zz = z * z;
    const float yz = z * y;
    const float xz = z * x;
    const float xy = y * x;
    const float wx = w * x;
    const double wy = static_cast<double>(w) * y;
    const double wz = static_cast<double>(w) * z;

    CSF_AT(float, matrix, 0x00) = g_TransitionOne - ((zz + yy) + (zz + yy));
    CSF_AT(float, matrix, 0x04) = static_cast<float>((xy + wz) + (xy + wz));
    CSF_AT(float, matrix, 0x08) = static_cast<float>((xz - wy) + (xz - wy));
    CSF_AT(float, matrix, 0x10) = static_cast<float>((xy - wz) + (xy - wz));
    CSF_AT(float, matrix, 0x14) = g_TransitionOne - ((zz + xx) + (zz + xx));
    CSF_AT(float, matrix, 0x18) = (wx + yz) + (wx + yz);
    CSF_AT(float, matrix, 0x20) = static_cast<float>((wy + xz) + (wy + xz));
    CSF_AT(float, matrix, 0x24) = (yz - wx) + (yz - wx);

    const float diagonalZ = g_TransitionOne - ((yy + xx) + (yy + xx));
    CSF_AT(std::uint32_t, matrix, 0x30) = 0;
    CSF_AT(std::uint32_t, matrix, 0x34) = 0;
    CSF_AT(std::uint32_t, matrix, 0x38) = 0;
    CSF_AT(std::uint32_t, matrix, 0x0C) = 3;
    CSF_AT(float, matrix, 0x28) = diagonalZ;

    CSF_AT(std::uint32_t, matrix, 0x30) = CSF_AT(std::uint32_t, keyframe, 0x18);
    CSF_AT(std::uint32_t, matrix, 0x34) = CSF_AT(std::uint32_t, keyframe, 0x1C);
    CSF_AT(std::uint32_t, matrix, 0x38) = CSF_AT(std::uint32_t, keyframe, 0x20);

    return matrixAddress;
}

HOOK(0x006C25E0, GfxAnimCompress_KeyframeToMatrix, "auto");
