#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Hud_CheckFlagAgainstGlobal, 0x006B2800, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, Water_DelegateResolve, 0x006FF440, (void* self, void* edx, uint32_t* a2, uint32_t* a3));

static int __fastcall C_00863678_ResolveOrDelegate(C_00863678* self, void* /*edx*/, uint32_t* a2, uint32_t* a3)
{
    if (Hud_CheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BBF18))))
    {
        *a3 = reinterpret_cast<uint32_t>(self);
        return self != nullptr;
    }

    Water_DelegateResolve(self, nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x0070AFD0, C_00863678_ResolveOrDelegate, "auto");
