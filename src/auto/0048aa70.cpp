#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GameConfig_IsResolvedEntryActive, 0x00755880, (int entry));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __stdcall CreateCandidateInteraction(void* a1, float arg4, int a3, int a2)
{
    if (!a1)
        return 0;
    if ((CSF_AT(int, a1, 0x170) & 0x2000) == 0 &&
        !(uint8_t)GameConfig_IsResolvedEntryActive(CSF_AT(int, a1, 0x50)))
        return 0;

    int result = 0;
    if (CHandleTable_Resolve(g_pCurrentScene, nullptr, a2) != (int)(intptr_t)a1) {
        int globalObj = GetValue8B0554();
        void* mgr = CSF_AT(void*, (void*)(intptr_t)globalObj, 0x1FC0);
        void** vtbl = CSF_AT(void**, mgr, 0);
        int entry = ((int(__fastcall*)(void*, void*, int))vtbl[2])(mgr, nullptr, a3);
        uint8_t* e = (uint8_t*)(intptr_t)entry;

        if (arg4 < CSF_AT(float, e, 0xEC)) {
            bool sameOwner = CSF_AT(int, a1, 0x78) == CSF_AT(int, (void*)(intptr_t)a2, 0x28);
            bool inRange = arg4 > CSF_AT(float, e, 0xE8) && arg4 < CSF_AT(float, e, 0xEC);
            if (CSF_AT(int, e, 0x74) == 10) {
                int g = GetGlobalValue_8BC1E0();
                inRange = arg4 < CSF_AT(float, (void*)(intptr_t)g, 0x1840);
            }
            if (!sameOwner && CSF_AT(int, (void*)(intptr_t)GetGlobalState_50D190(), 0xCC) != 0)
                return 3;
            if (inRange)
                return 2;
            if (arg4 < CSF_AT(float, e, 0xE8))
                return 1;
        }
    }
    return result;
}

HOOK(0x0048AA70, CreateCandidateInteraction, "auto");
