#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

using QuaternionAxisCycle = std::int32_t[3];
CSF_GLOBAL(QuaternionAxisCycle, g_QuaternionAxisCycle, 0x008A7DE0);

static void __fastcall Matrix_ExtractQuaternion(
    float* self, void* /*edx*/, const void** source)
{
    float basis[9];

    std::uint32_t* matrix = RwMatrixCreate();
    std::memcpy(matrix, *source, 0x40u);
    basis[0] = CSF_AT(float, matrix, 0x00);
    basis[1] = CSF_AT(float, matrix, 0x04);
    basis[2] = CSF_AT(float, matrix, 0x08);
    RwMatrixDestroy(matrix);

    matrix = RwMatrixCreate();
    std::memcpy(matrix, *source, 0x40u);
    basis[3] = CSF_AT(float, matrix, 0x10);
    basis[4] = CSF_AT(float, matrix, 0x14);
    basis[5] = CSF_AT(float, matrix, 0x18);
    RwMatrixDestroy(matrix);

    matrix = RwMatrixCreate();
    std::memcpy(matrix, *source, 0x40u);
    basis[6] = CSF_AT(float, matrix, 0x20);
    basis[7] = CSF_AT(float, matrix, 0x24);
    basis[8] = CSF_AT(float, matrix, 0x28);
    RwMatrixDestroy(matrix);

    matrix = RwMatrixCreate();
    std::memcpy(matrix, *source, 0x40u);
    RwMatrixDestroy(matrix);

    const double trace = basis[8] + basis[4] + basis[0];
    if (trace <= g_fZero)
    {
        int largestAxis = basis[4] > basis[0];
        if (basis[8] > basis[4 * largestAxis])
            largestAxis = 2;

        const int nextAxis = g_QuaternionAxisCycle[largestAxis];
        const int lastAxis = g_QuaternionAxisCycle[nextAxis];

        float* components[3] = {
            &CSF_AT(float, self, 0x00),
            &CSF_AT(float, self, 0x04),
            &CSF_AT(float, self, 0x08)
        };

        const double diagonalDifference =
            static_cast<double>(basis[4 * largestAxis]) -
            basis[4 * nextAxis] - basis[4 * lastAxis];
        const double root =
            std::sqrt(diagonalDifference + g_TransitionOne);

        *components[largestAxis] =
            static_cast<float>(g_HalfDurationScale * root);

        const double scale = g_HalfDurationScale / root;
        CSF_AT(float, self, 0x0C) = static_cast<float>(
            (basis[3 * nextAxis + lastAxis] -
             basis[3 * lastAxis + nextAxis]) * scale);

        *components[nextAxis] = static_cast<float>(
            (basis[3 * largestAxis + nextAxis] +
             basis[3 * nextAxis + largestAxis]) * scale);

        *components[lastAxis] = static_cast<float>(
            (basis[3 * largestAxis + lastAxis] +
             basis[3 * lastAxis + largestAxis]) * scale);
    }
    else
    {
        const double root = std::sqrt(trace + g_TransitionOne);
        CSF_AT(float, self, 0x0C) =
            static_cast<float>(g_HalfDurationScale * root);

        const double scale = g_HalfDurationScale / root;
        CSF_AT(float, self, 0x00) =
            static_cast<float>((basis[5] - basis[7]) * scale);
        CSF_AT(float, self, 0x04) =
            static_cast<float>((basis[6] - basis[2]) * scale);
        CSF_AT(float, self, 0x08) =
            static_cast<float>((basis[1] - basis[3]) * scale);
    }
}

HOOK(0x006AEBC0, Matrix_ExtractQuaternion, "auto");
