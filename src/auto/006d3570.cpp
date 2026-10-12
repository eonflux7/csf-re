#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void* edx));

// Fills the object with two zero-based vector slots (X,Y,Z then X,Y,Z from the
// zero-vector globals), then normalizes it. Returns the object. The stack
// arguments a2/a3 are unused by the original.
static C_00833b88* __stdcall GfxCamera_InitZeroVectorPairAndNormalize(C_00833b88* a1, int a2, int a3) {
    (void)a2;
    (void)a3;

    float zeroY = g_ZeroVectorY;
    float zeroX = g_ZeroVectorX;
    float zeroZ = g_ZeroVectorZ;

    CSF_AT(uint32_t, a1, 0x00) = 0;
    CSF_AT(uint32_t, a1, 0x04) = 0;
    CSF_AT(uint32_t, a1, 0x08) = 0;
    CSF_AT(uint32_t, a1, 0x0C) = 0;
    CSF_AT(uint32_t, a1, 0x10) = 0;
    CSF_AT(uint32_t, a1, 0x14) = 0;

    CSF_AT(float, a1, 0x00) = zeroX;
    CSF_AT(float, a1, 0x04) = zeroY;
    CSF_AT(float, a1, 0x08) = zeroZ;
    CSF_AT(float, a1, 0x0C) = zeroX;
    CSF_AT(float, a1, 0x10) = zeroY;
    CSF_AT(float, a1, 0x14) = zeroZ;

    NormalizeVector3(a1, nullptr);
    return a1;
}

HOOK(0x006D3570, GfxCamera_InitZeroVectorPairAndNormalize, "auto");
