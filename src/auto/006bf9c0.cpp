#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, RwFrameUpdateObjects, 0x006236A0, (int a1));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(float*, __fastcall, GfxMatrix_GetEulerAngles, 0x006AE3A0, (float** self, void*, float* angles));

CSF_GLOBAL(float, g_DegreesPerRadian, 0x0086061C);

// Copies a 4x4 matrix (0x40 bytes) into the frame's matrix slot, derives Euler
// angles in degrees into the object's angle fields, refreshes the frame's
// objects, and destroys the temporary matrix when the callee flagged it.
static char __fastcall C_00860700_SetMatrixAndUpdateEulerDegrees(C_00860700* self, void* /*edx*/, const void** a2)
{
    const void* sourceMatrix = *a2;

    // [ebp-8]: matrix pointer (first 4 bytes) + flag byte (at +4)
    alignas(8) uint8_t matrixHolder[8];
    // [ebp-0x20]: scratch space for the Euler angles
    alignas(8) uint8_t eulerBuffer[0x18];

    uint8_t* matrixDest = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_000c + 16u));
    CSF_AT(uint8_t*, matrixHolder, 0) = matrixDest;
    CSF_AT(char, matrixHolder, 4) = 0;
    std::memcpy(matrixDest, sourceMatrix, 0x40);

    float* eulerAngles = GfxMatrix_GetEulerAngles(
        reinterpret_cast<float**>(matrixHolder), nullptr, reinterpret_cast<float*>(eulerBuffer));

    self->field_0014 = eulerAngles[0] * g_DegreesPerRadian;
    CSF_AT(float, self, 0x18) = eulerAngles[1] * g_DegreesPerRadian;
    uint32_t frameHandle = self->field_000c;
    float angleZ = eulerAngles[2] * g_DegreesPerRadian;
    CSF_AT(float, self, 0x1c) = angleZ;

    RwFrameUpdateObjects(static_cast<int>(frameHandle));

    char result = CSF_AT(char, matrixHolder, 4);
    if (result) {
        uint8_t* matrixToDestroy = CSF_AT(uint8_t*, matrixHolder, 0);
        return static_cast<char>(RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(matrixToDestroy)));
    }
    return result;
}

HOOK(0x006BF9C0, C_00860700_SetMatrixAndUpdateEulerDegrees, "auto");
