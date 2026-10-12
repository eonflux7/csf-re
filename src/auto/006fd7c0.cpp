#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Hud_CheckFlagAgainstGlobal, 0x006B2800, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, C_00862fcc_DelegateResolve, 0x006D3FB0, (void* self, void* edx, uint32_t* a2, int* a3));

static int __fastcall C_00862fcc_ResolveFlagOrDelegate(C_00862fcc* self, void* /*edx*/, uint32_t* a2, int* a3)
{
    if (static_cast<uint8_t>(Hud_CheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BBB3C)))))
    {
        *a3 = (int)self;
        return self != nullptr;
    }

    C_00862fcc_DelegateResolve(self, nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x006FD7C0, C_00862fcc_ResolveFlagOrDelegate, "auto");
