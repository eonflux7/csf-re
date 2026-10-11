#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RwMatrixUpdate, 0x00624620, (int a1));

// Copies a 3-dword vector into the matrix at +0x20, then calls RwMatrixUpdate on the matrix.
// self is the object whose first dword points to the matrix; src is the 3-dword vector.
static int __fastcall GfxMatrix_SetScaleAndUpdate(void* self, void* /*edx*/, const uint32_t* src) {
    uint32_t matrixAddress = CSF_AT(uint32_t, self, 0);
    uint8_t* matrix = reinterpret_cast<uint8_t*>(matrixAddress);

    CSF_AT(uint32_t, matrix, 0x20) = src[0];
    CSF_AT(uint32_t, matrix, 0x24) = src[1];
    CSF_AT(uint32_t, matrix, 0x28) = src[2];

    return RwMatrixUpdate(static_cast<int>(CSF_AT(uint32_t, self, 0)));
}

HOOK(0x006AD9B0, GfxMatrix_SetScaleAndUpdate, "auto");
