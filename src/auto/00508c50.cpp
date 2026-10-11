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

CSF_FUNC(unsigned int, __cdecl, Hud_WriteNormalizedScreenSize, 0x0042DE60, (float* out));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, S_00508360_Reset, 0x00508360, (void* self, void* edx));
CSF_FUNC(char, __fastcall, S_00508AF0_CanActivate, 0x00508AF0, (void* self, void* edx, int a2));

static bool __fastcall S_00508C50_TryBindMenuObject(S_00508360* self, void* /*edx*/, int a2)
{
    if (S_00508AF0_CanActivate(self, nullptr, a2))
    {
        Hud_WriteNormalizedScreenSize(reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(self) + 4));

        int globalInt = GetGlobalInt();
        void* globalObject = reinterpret_cast<void*>(static_cast<uintptr_t>(globalInt));
        void* menuObject = MenuControls_ResolveObject(globalObject, nullptr);

        uint32_t menuValue = ((uint32_t(__fastcall*)(void*, void*))CSF_AT(void**, menuObject, 0)[22])(menuObject, nullptr);
        bool hasMenuValue = menuValue != 0;
        self->field_0030 = menuValue;
        if (!menuValue)
        {
            S_00508360_Reset(self, nullptr);
        }
        return hasMenuValue;
    }

    S_00508360_Reset(self, nullptr);
    return false;
}

HOOK(0x00508C50, S_00508C50_TryBindMenuObject, "auto");
