#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x00521BB0 - returns the global object whose address is passed as `this`
// to the update method below (value returned in eax, __cdecl, ret 0).
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x521BB0, ());

// 0x005225D0 - __thiscall method invoked on the value returned above
// (ecx = this, no stack args -> ret 0). Return value is ignored.
CSF_FUNC(int, __fastcall, Menu_5225D0, 0x5225D0, (void* self, void* /*edx*/));

// 0x00525130 - returns the object stored in g_008b0678 (__cdecl, ret 0).
CSF_FUNC(int, __cdecl, Get_008b0678, 0x525130, ());

// 0x00525DD0 - __thiscall setter taking one stack argument (ret 4, pops one dword).
CSF_FUNC(int, __fastcall, MenuMissionInfo_SetGlobalIndex, 0x525DD0, (void* self, void* /*edx*/, int value));

CSF_GLOBAL(int, g_008b0678, 0x8B0678);

static int __cdecl MenuMissionInfo_InitGlobalState()
{
    // v0 = GetGlobalValue_521BB0(); sub_5225D0(v0);
    Menu_5225D0((void*)GetGlobalValue_521BB0(), nullptr);

    // mov eax, [0x8b0678]; test eax, eax; je -> ret
    int storedIndex = g_008b0678;
    if (storedIndex)
    {
        // push 0x60; call 0x525130; mov ecx, eax; call 0x525dd0
        return MenuMissionInfo_SetGlobalIndex((void*)Get_008b0678(), nullptr, 96);
    }
    return storedIndex;
}

HOOK(0x53D500, MenuMissionInfo_InitGlobalState, "auto");
