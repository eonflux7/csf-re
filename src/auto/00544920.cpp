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

// Original's string/data global: first pointer loaded from 0x899AF4 (g_pszMenu_P_MeJueg).
CSF_GLOBAL(const char*, g_pszMenu_P_MeJueg, 0x00899AF4);

// Resolves the active child panel from the menu object (__thiscall, no stack args).
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));

// Command dispatcher for CMenuSaveGame (__thiscall, 2 stack args, ret 8).
static int __fastcall CMenuSaveGame_HandleCommand(int* self, void* /*edx*/, int a2, int a3)
{
    if (a2 == 1 || a2 == 14)
    {
        int state = CSF_AT(int, self, 0x4C);
        if (state != -1)
            return state;

        int* esi = self - 1;
        CSF_AT(int, esi, 0x50) = 6;

        void* obj = CSF_AT(void*, esi, 0x40);
        ((int(__fastcall*)(void*, void*, void*, int))(CSF_AT(void**, obj, 0)[24]))(obj, nullptr, esi, 0);

        obj = CSF_AT(void*, esi, 0x40);
        return ((int(__fastcall*)(void*, void*, const char*))(CSF_AT(void**, obj, 0)[22]))(
            obj, nullptr, g_pszMenu_P_MeJueg);
    }

    int result = a2 - 50;
    if (a2 == 50)
    {
        result = a3;
        void* panelArg = reinterpret_cast<void*>(static_cast<uintptr_t>(a3));
        if (CSF_AT(uint8_t, panelArg, 0x38))
        {
            void* activeChild = GetReferencedObject(self, nullptr);
            return ((int(__fastcall*)(void*, void*))(CSF_AT(void**, activeChild, 0)[7]))(activeChild, nullptr);
        }
    }
    return result;
}

HOOK(0x00544920, CMenuSaveGame_HandleCommand, "auto");
