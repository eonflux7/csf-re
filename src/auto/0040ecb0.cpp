#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

// Fills two consecutive 3-float vectors (at offsets 0 and 12) with the zero vector
// constants and returns the original pointer.
static float* __fastcall AiStatic_InitializeSegment(float* self, void* /*edx*/)
{
    float* result = self;

    self[0] = g_ZeroVectorX;
    self[1] = g_ZeroVectorY;
    self[2] = g_ZeroVectorZ;

    float* second = self + 3;
    second[0] = g_ZeroVectorX;
    second[1] = g_ZeroVectorY;
    second[2] = g_ZeroVectorZ;

    return result;
}

HOOK(0x0040ECB0, AiStatic_InitializeSegment, "auto");
