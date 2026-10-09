#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Clamps a2 to the signed int stored at offset +16 of the 20-byte entry
// number a3 in the table pointed to by this->field_0034.
// Original: __thiscall, 2 stack args, ret 8.
static int __fastcall S_0042eb60_ClampToEntryLimit(S_0042eb60* self, void* /*edx*/, int value, int entryIndex)
{
    uint8_t* tableBase = reinterpret_cast<uint8_t*>(self->field_0034);
    uint8_t* entry = tableBase + 20 * entryIndex;
    int entryLimit = CSF_AT(int, entry, 0x10);

    if (value < entryLimit)
        return value;
    return entryLimit;
}

HOOK(0x0042EB60, S_0042eb60_ClampToEntryLimit, "auto");
