#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static float* __stdcall CopyZeroVector(float* destination, int /*unused*/)
{
    destination[0] = g_ZeroVectorX;
    destination[1] = g_ZeroVectorY;
    destination[2] = g_ZeroVectorZ;
    return destination;
}

HOOK(0x0048ED80, CopyZeroVector, "auto");
