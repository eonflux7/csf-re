#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HudCheckFlagAgainstGlobal, 0x006B2800, (uint32_t* self, void* /*edx*/, uint32_t* other));
CSF_FUNC(int, __fastcall, C_008620c8_ResolveOrDelegate, 0x00708090, (C_008620c8* self, void* /*edx*/, uint32_t* identifier, int* delegateOut));

static int __fastcall C_00862110_ResolveOrDelegate(C_00862110* self, void* /*edx*/, uint32_t* a2, int* a3)
{
    if (HudCheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BBAF0))))
    {
        *a3 = (int)self;
        return self != nullptr;
    }

    C_008620c8_ResolveOrDelegate(reinterpret_cast<C_008620c8*>(self), nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x006EEAB0, C_00862110_ResolveOrDelegate, "auto");
