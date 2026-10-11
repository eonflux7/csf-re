#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Hud_CheckFlagAgainstGlobal, 0x006B2800, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, Water_DelegateResolve, 0x006FF440, (void* self, void* edx, uint32_t* a2, uint32_t* a3));

static int __fastcall C_00863600_ResolveOrDelegate(C_00863600* self, void* /*edx*/, uint32_t* a2, uint32_t* a3)
{
    if (Hud_CheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BBF14))))
    {
        *a3 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(self));
        return self != nullptr;
    }

    Water_DelegateResolve(self, nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x0070A810, C_00863600_ResolveOrDelegate, "auto");
