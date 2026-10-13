#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Hud_CheckFlagAgainstGlobal, 0x006B2800, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, C_00861a60_DelegateResolve, 0x006B1FB0, (void* self, void* edx, void* a2, uint32_t* a3));

static int __fastcall C_00861b40_ResolveOrDelegate(C_00861b40* self, void* /*edx*/, uint32_t* a2, uint32_t* a3)
{
    if (Hud_CheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BBAA4))))
    {
        *a3 = reinterpret_cast<uint32_t>(self);
        return self != nullptr;
    }

    C_00861a60_DelegateResolve(self, nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x006E3EC0, C_00861b40_ResolveOrDelegate, "auto");
