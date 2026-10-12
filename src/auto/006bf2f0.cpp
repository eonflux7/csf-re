#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// RwMatrixOrthoNormalize: __cdecl, caller pops the two arguments.
CSF_FUNC(int, __cdecl, RwMatrixOrthoNormalize, 0x006246A0, (int a1, int a2));

// Orthonormalizes the matrix embedded at offset 0x10 of the object pointed to by the
// field at offset 0x0C. The original reads the field once and passes the same address twice.
static int __fastcall GfxFrames_OrthoNormalizeOwnMatrix(void* self, void* /*edx*/) {
    uint32_t matrixAddress = CSF_AT(uint32_t, self, 0x0C) + 0x10;
    return RwMatrixOrthoNormalize(static_cast<int>(matrixAddress), static_cast<int>(matrixAddress));
}

HOOK(0x006BF2F0, GfxFrames_OrthoNormalizeOwnMatrix, "auto");
