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

// Original functions called by the continue handler, bound by address.
CSF_FUNC(int, __cdecl, GetGlobalValue, 0x521BB0, ());
CSF_FUNC(int, __fastcall, ProcessGlobalValueHandle, 0x521F80, (void* self, void* edx));
CSF_FUNC(void, __fastcall, ApplyGlobalValueToSubsystem, 0x5233D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuScreen_CloseWithCode, 0x539D10, (void* self, void* edx, int code));

// Global string pointer, value is the const char* stored at the address.
CSF_GLOBAL(const char*, g_pszMenu_P_Juego, 0x899AF0);

// Button handler for "Continue" in the pause menu.
static int __fastcall CMenuPause_OnContinueClicked(CMenuPause* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_0010);
    if (result)
    {
        int firstGlobal = GetGlobalValue();
        ProcessGlobalValueHandle(reinterpret_cast<void*>(static_cast<uintptr_t>(firstGlobal)), nullptr);

        int secondGlobal = GetGlobalValue();
        ApplyGlobalValueToSubsystem(reinterpret_cast<void*>(static_cast<uintptr_t>(secondGlobal)), nullptr);

        // Reload the owner pointer from the field after the calls above.
        void* owner = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0010));
        void** ownerVtable = *reinterpret_cast<void***>(owner);
        using OwnerMethod = void*(__fastcall*)(void*, void*, const char*);
        void* notifyTarget = reinterpret_cast<OwnerMethod>(ownerVtable[0x5C / 4])(
            owner, nullptr, g_pszMenu_P_Juego);

        if (notifyTarget)
        {
            void** targetVtable = *reinterpret_cast<void***>(notifyTarget);
            using TargetMethod = void(__fastcall*)(void*, void*, int);
            reinterpret_cast<TargetMethod>(targetVtable[0x54 / 4])(notifyTarget, nullptr, 1);
        }

        return CMenuScreen_CloseWithCode(self, nullptr, 0);
    }
    return result;
}

HOOK(0x0053A280, CMenuPause_OnContinueClicked, "auto");
