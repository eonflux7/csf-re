#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(const char*, g_pszMenu_P_Tactic, 0x00899B14);

CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, UmsVirtualProcessor_GetDefaultDestination, 0x004FE4A0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobal8afc7c, 0x0043E010, ());
CSF_FUNC(int*, __fastcall, Campaign_FlushPendingListEntry, 0x00441960, (void* self, void*));

static int __fastcall SceneResult_InstanceTacticMenu(void* self, void* /*edx*/)
{
    void* manager = CSF_AT(void*, self, 0x18);
    void** vtbl = CSF_AT(void**, manager, 0);
    int menuHandle = ((int(__fastcall*)(void*, void*, const char*, int))vtbl[23])(manager, nullptr, g_pszMenu_P_Tactic, 1);
    int result = ((int(__fastcall*)(void*, void*, int))vtbl[29])(manager, nullptr, menuHandle);

    if (!static_cast<uint8_t>(result))
    {
        int globalValue = GetGlobalValue_8B0568();
        void* destination = UmsVirtualProcessor_GetDefaultDestination(
            reinterpret_cast<void*>(static_cast<uintptr_t>(globalValue)), nullptr);
        if (CSF_AT(int, destination, 0x1C))
        {
            int globalHandle = GetGlobal8afc7c();
            Campaign_FlushPendingListEntry(
                reinterpret_cast<void*>(static_cast<uintptr_t>(globalHandle)), nullptr);
        }

        void* manager2 = CSF_AT(void*, self, 0x18);
        int menuInstance = ((int(__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, manager2, 0)[23])(manager2, nullptr, g_pszMenu_P_Tactic);
        result = menuInstance;

        if (menuInstance != 0)
        {
            CSF_AT(void*, self, 0x154) = self;
            uint8_t* subObject = static_cast<uint8_t*>(self) + 0x154;
            CSF_AT(uint8_t, self, 0x183) = 0;
            CSF_AT(uint8_t, self, 0x182) = 0;
            CSF_AT(uint8_t, self, 0x188) = 0;
            CSF_AT(uint8_t, self, 0x158) = 0;
            CSF_AT(void*, reinterpret_cast<void*>(static_cast<uintptr_t>(menuInstance)), 0x14) = subObject;

            void* manager3 = CSF_AT(void*, self, 0x18);
            result = ((int(__fastcall*)(void*, void*, const char*))
                CSF_AT(void**, manager3, 0)[22])(manager3, nullptr, g_pszMenu_P_Tactic);
        }

        CSF_AT(int, self, 0x130) = 5;
    }

    return result;
}

HOOK(0x00536010, SceneResult_InstanceTacticMenu, "auto");
