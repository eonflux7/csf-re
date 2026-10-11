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

CSF_FUNC(int, __fastcall, GfxMatrix_RotateZAxisRadians, 0x006ADBE0, (int* self, void*, int a2, float a3));
CSF_GLOBAL(float, g_RadToDegFactor, 0x0082D7C4);

// Rotates the Z-axis matrix stored in the entry at index entryIndex of the table
// at self+0x0C, then, if the entry's first slot holds an object, calls its virtual
// slot 18 with the rotated matrix and the angle in degrees.
static int __fastcall RotateEntryZAndNotify(uint32_t* self, void* /*edx*/, int entryIndex, float angleRadians)
{
    uint8_t* table = CSF_AT(uint8_t*, self, 0x0C);
    uint8_t* entry = table + 28 * entryIndex;

    // Copy the 3-dword matrix from the object referenced at entry+8 (+16 bytes).
    int* matrixSrc = reinterpret_cast<int*>(CSF_AT(uint8_t*, entry, 8) + 16);
    int matrix[3];
    matrix[0] = matrixSrc[0];
    matrix[1] = matrixSrc[1];
    matrix[2] = matrixSrc[2];

    GfxMatrix_RotateZAxisRadians(reinterpret_cast<int*>(entry + 8), nullptr,
                                 static_cast<int>(reinterpret_cast<uintptr_t>(matrix)), angleRadians);

    // Re-read the table pointer after the call; the callee may have changed it.
    uint8_t* currentTable = CSF_AT(uint8_t*, self, 0x0C);
    void* notifyTarget = CSF_AT(void*, currentTable, 28 * entryIndex);
    if (notifyTarget == nullptr) {
        return static_cast<int>(reinterpret_cast<uintptr_t>(currentTable));
    }

    float degrees = angleRadians * g_RadToDegFactor;
    return ((int(__fastcall*)(void*, void*, int*, float))CSF_AT(void**, notifyTarget, 0)[0x48 / 4])(
        notifyTarget, nullptr, matrix, degrees);
}

HOOK(0x00408200, RotateEntryZAndNotify, "auto");
