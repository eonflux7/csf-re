#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(float*, __fastcall, GfxMatrix_GetEulerAngles, 0x006AE3A0, (float** self, void* /*edx*/, float* angles));

CSF_GLOBAL(float, g_RadiansToDegreesFactor, 0x0086061C);

// Stores the frame's rotation as Euler angles in degrees, taken from the matrix
// that lives at a2 + 16. The matrix is destroyed afterwards if the callee flagged it.
static char __fastcall GfxFrame_InitRotationDegreesFromMatrix(void* self, void* /*edx*/, int matrixBase)
{
    CSF_AT(int, self, 0x0C) = matrixBase;
    CSF_AT(std::uint8_t, self, 0x10) = 0;

    // Two-dword stack slot: [0] = matrix pointer (a2 + 16), [4] = destroy flag byte.
    alignas(8) std::uint8_t matrixSlot[8];
    CSF_AT(int, matrixSlot, 0) = matrixBase + 16;
    CSF_AT(std::uint8_t, matrixSlot, 4) = 0;

    // Angle output buffer (room 0x18 bytes on the original frame).
    alignas(4) std::uint8_t anglesBytes[0x18];
    float* angles = reinterpret_cast<float*>(anglesBytes);

    float* eulerAngles = GfxMatrix_GetEulerAngles(reinterpret_cast<float**>(matrixSlot), nullptr, angles);

    CSF_AT(float, self, 0x14) = eulerAngles[0] * g_RadiansToDegreesFactor;
    CSF_AT(float, self, 0x18) = eulerAngles[1] * g_RadiansToDegreesFactor;
    CSF_AT(float, self, 0x1C) = eulerAngles[2] * g_RadiansToDegreesFactor;

    char result = static_cast<char>(CSF_AT(std::uint8_t, matrixSlot, 4));
    if (result) {
        int matrixPointer = CSF_AT(int, matrixSlot, 0);
        return static_cast<char>(RwMatrixDestroy(
            reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(matrixPointer))));
    }
    return result;
}

HOOK(0x006BF380, GfxFrame_InitRotationDegreesFromMatrix, "auto");
