#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static float* __stdcall AudioZeroVector_CopyToOutput(float* outVector)
{
    outVector[0] = g_ZeroVectorX;
    outVector[1] = g_ZeroVectorY;
    outVector[2] = g_ZeroVectorZ;
    return outVector;
}

HOOK(0x0070EBF0, AudioZeroVector_CopyToOutput, "auto");
