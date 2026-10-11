#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, _rwGetNumEngineInstances, 0x007DB820, ());
CSF_FUNC(int, __cdecl, RwResourcesEmptyArena, 0x007DB8B0, ());
CSF_FUNC(void, __cdecl, RwFreeListForAllFreeLists, 0x007DE590, (int (__cdecl* a1)(int*, int), int a2));
CSF_FUNC(int, __cdecl, sub_7EF3C0, 0x007EF3C0, (uint32_t* a1, int a2));

using FreeListCallback = int(__cdecl*)(int*, int);

// __thiscall C_00863ba8::<method> -> free function; ecx = this, edx unused. Callee pops 0 (ret 0).
static int __fastcall C_00863ba8_ReleaseEffectDictionaries(C_00863ba8* self, void* /*edx*/)
{
    int result = _rwGetNumEngineInstances();
    if (!result)
        return result;

    RwFreeListForAllFreeLists(reinterpret_cast<FreeListCallback>(CSF_ADDR_FN(0x0070F5F0)), 0);
    RwResourcesEmptyArena();
    RwFreeListForAllFreeLists(reinterpret_cast<FreeListCallback>(CSF_ADDR_FN(0x0070F610)), 0);
    RwResourcesEmptyArena();

    for (int i = 0; i < static_cast<int>(self->field_0010); ++i)
    {
        uint32_t entry = reinterpret_cast<uint32_t*>(self->field_000c)[i];
        if (entry)
            sub_7EF3C0(reinterpret_cast<uint32_t*>(entry), 0);
    }
    return RwResourcesEmptyArena();
}

HOOK(0x00710D80, C_00863ba8_ReleaseEffectDictionaries, "auto");
