#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(char, __fastcall, WeaponFirearms_CheckState, 0x00681D60, (void* self, void* edx));

static int __fastcall C_008317c8_IsStateBlocked(C_008317c8* self, void* /*edx*/)
{
    const bool blocked =
        C_008319c0_GetSubObjectField74(
            reinterpret_cast<C_008319c0*>(self), nullptr) == 8 ||
        self->field_0070 != 0 ||
        (self->field_006c & 0x80000000) != 0 ||
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(reinterpret_cast<std::uint8_t*>(self) + 0x64)) == 0;

    return WeaponFirearms_CheckState(self, nullptr) && blocked;
}

HOOK(0x00478A20, C_008317c8_IsStateBlocked, "auto");
