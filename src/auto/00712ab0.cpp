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

CSF_FUNC(int, __cdecl, _rwGetNumEngineInstances, 0x007DB820, ());
CSF_FUNC(void, __cdecl, RwFreeListForAllFreeLists, 0x007DE590, (int (__cdecl* a1)(int*, int), int a2));

CSF_GLOBAL(float, flt_8A85D8, 0x008A85D8);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

// Stored into the RW free-list iteration as the per-list callback.
static int __cdecl Audio_FreeListCallbackAddr_Unused() { return 0; }

static int Audio_SetClampedUnitGain(float a1)
{
    if (a1 <= g_TransitionOne)
    {
        if (a1 < g_fZero)
            a1 = g_fZero;
    }
    else
    {
        a1 = g_TransitionOne;
    }

    flt_8A85D8 = a1;

    int engineInstanceCount = _rwGetNumEngineInstances();
    if (engineInstanceCount)
    {
        RwFreeListForAllFreeLists(
            reinterpret_cast<int (__cdecl*)(int*, int)>(CSF_ADDR_FN(0x007128A0)),
            static_cast<int>(reinterpret_cast<uintptr_t>(&a1)));
        return engineInstanceCount;
    }
    return engineInstanceCount;
}

HOOK(0x00712AB0, Audio_SetClampedUnitGain, "auto");
