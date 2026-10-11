#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_AxisZ, 0x008A7D84);
CSF_GLOBAL(float, g_AxisZ_Y, 0x008A7D88);
CSF_GLOBAL(float, g_AxisZ_Z, 0x008A7D8C);

static float* __stdcall CopyAxisZVector(float* destination)
{
    destination[0] = g_AxisZ;
    destination[1] = g_AxisZ_Y;
    destination[2] = g_AxisZ_Z;
    return destination;
}

HOOK(0x0070DE00, CopyAxisZVector, "auto");
