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

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

// Blends the two input scales into the matrix's first and second row offsets,
// hands the resulting vector to the embedded object, and destroys the matrix
// if the callee flagged it as owned.
static char __fastcall CCamera_ApplyBlendedMatrixOffset(CCamera* self, void* /*edx*/, float a2, float a3)
{
    uint32_t* embeddedObject = &self->field_0010;

    // Ask the embedded object (slot 0x90) for its current matrix.
    // The callee writes a matrix pointer and an ownership flag into the 8-byte slot.
    alignas(8) uint8_t matrixOutBytes[8] = {};
    float** matrixOut = reinterpret_cast<float**>(matrixOutBytes);
    ((void(__fastcall*)(void*, void*, float**))CSF_AT(void**, embeddedObject, 0)[0x90 / 4])(
        embeddedObject, nullptr, matrixOut);

    float* matrix = *matrixOut;

    alignas(8) float resultVec[6] = {};
    resultVec[0] = a2 * matrix[0] + a3 * matrix[4];
    resultVec[1] = a2 * matrix[1] + a3 * matrix[5];
    resultVec[2] = a2 * matrix[2] + a3 * matrix[6];

    // Push the blended vector to the embedded object (slot 0x20).
    ((void(__fastcall*)(void*, void*, float*))CSF_AT(void**, embeddedObject, 0)[0x20 / 4])(
        embeddedObject, nullptr, resultVec);

    char ownsMatrix = static_cast<char>(matrixOutBytes[4]);
    if (ownsMatrix)
    {
        return static_cast<char>(RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(*matrixOut)));
    }
    return ownsMatrix;
}

HOOK(0x006D2CF0, CCamera_ApplyBlendedMatrixOffset, "auto");
