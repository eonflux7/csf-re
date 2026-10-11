#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __cdecl FxParticleManager_CompareSortKeys(float* left, float* right)
{
    if (CSF_AT(float, left, 0x14) < CSF_AT(float, right, 0x14))
        return -1;

    return CSF_AT(float, left, 0x14) > CSF_AT(float, right, 0x14) ? 1 : 0;
}

HOOK(0x0051EAE0, FxParticleManager_CompareSortKeys, "auto");
