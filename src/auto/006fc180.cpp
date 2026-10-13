#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HudCheckFlagAgainstGlobal, 0x006B2800, (uint32_t* self, void* edx, uint32_t* other));
CSF_FUNC(int, __fastcall, C_00862e88_DelegateResolve, 0x00709680, (void* self, void* edx, uint32_t* a2, int* a3));

static int __fastcall C_00862e88_ResolveOrDelegate(C_00862e88* self, void* /*edx*/, uint32_t* a2, int* a3)
{
    if (HudCheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BBB34))))
    {
        *a3 = (int)self;
        return self != nullptr;
    }

    C_00862e88_DelegateResolve(self, nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x006FC180, C_00862e88_ResolveOrDelegate, "auto");
