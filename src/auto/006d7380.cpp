#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Hud_CheckFlagAgainstGlobal, 0x006B2800, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, C_008611d4_DelegateResolve, 0x00700F90, (void* self, void* edx, uint32_t* a2, int* a3));

static int __fastcall C_008611d4_ResolveOrDelegate(
    C_008611d4* self, void* /*edx*/, uint32_t* a2, int* a3)
{
    if (static_cast<uint8_t>(Hud_CheckFlagAgainstGlobal(
            a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BB70C)))))
    {
        *a3 = reinterpret_cast<int>(self);
        return self != nullptr;
    }

    C_008611d4_DelegateResolve(self, nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x006D7380, C_008611d4_ResolveOrDelegate, "auto");
