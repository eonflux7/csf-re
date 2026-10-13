#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HudCheckFlagAgainstGlobal, 0x006B2800, (uint32_t* self, void* , uint32_t* other));
CSF_FUNC(int, __fastcall, C_008305f8_ResolveOrDelegate_6C4540, 0x006C4540, (C_008305f8* self, void* , uint32_t* a2, int* a3));

// Checks the HUD flag of a2 against the global at 0x8BBAEC. If set, stores this into *a3
// and returns whether this is non-null; otherwise delegates to 0x6C4540 and returns whether *a3 is non-null.
static int __fastcall C_008305f8_ResolveOrDelegate(C_008305f8* self, void* /*edx*/, uint32_t* a2, int* a3)
{
    if (HudCheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BBAEC))))
    {
        *a3 = (int)self;
        return self != nullptr;
    }

    C_008305f8_ResolveOrDelegate_6C4540(self, nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x006EF290, C_008305f8_ResolveOrDelegate, "auto");
