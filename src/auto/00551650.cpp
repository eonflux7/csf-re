#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Reads the spawn-owner pointer at +0xC8C, follows owner->+0x18 to a
// sub-record whose flag table sits at base+0x443, and returns that flag
// as a sign-extended byte (0 when any link in the chain is null).
static int __fastcall MpSpawn_QueryFlagForGlobal(void* self, void* /*edx*/)
{
    uintptr_t owner = CSF_AT(uint32_t, self, 0xC8C);
    uintptr_t flagBase = 0;

    if (owner != 0)
    {
        uintptr_t sub = CSF_AT(uint32_t, reinterpret_cast<void*>(owner), 0x18);
        if (sub != 0)
            flagBase = sub - 0x6B0;
    }

    int result = 0;
    if (flagBase != 0)
        result = CSF_AT(int8_t, reinterpret_cast<void*>(flagBase), 0x443);

    return result;
}

HOOK(0x551650, MpSpawn_QueryFlagForGlobal, "auto");
