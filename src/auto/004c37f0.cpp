#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Grows the record array of S_004bdcd0 (returns nothing useful to us; result is ignored here).
CSF_FUNC(std::uint32_t*, __fastcall, S_004bdcd0_GrowCapacity, 0x004BDCD0, (S_004bdcd0* self, void* , int additionalCapacity));

// Appends one 52-byte record (13 dwords, copying all but dwords 3 and 9) to the array at self->field_0000.
// Grows the array first when full; a zero capacity hint falls back to self->field_000c (grow step).
// Stack args: source record pointer, capacity hint. Callee pops 8 (__thiscall-style with two stack args).
static std::uint32_t* __fastcall S_004bdcd0_AppendRecord(S_004bdcd0* self, void* /*edx*/, std::uint32_t* source, int capacityHint)
{
    if (self->field_0008 >= self->field_0004)
    {
        int growBy = capacityHint;
        if (growBy == 0)
            growBy = static_cast<int>(self->field_000c);
        S_004bdcd0_GrowCapacity(self, nullptr, growBy);
    }

    // Re-read count and base after a possible grow.
    std::uint32_t index = self->field_0008;
    std::uint32_t* base = reinterpret_cast<std::uint32_t*>(self->field_0000);
    std::uint32_t* result = reinterpret_cast<std::uint32_t*>(
        reinterpret_cast<std::uint8_t*>(base) + 52u * index);
    self->field_0008 = index + 1;

    result[0] = source[0];
    result[1] = source[1];
    result[2] = source[2];
    result[4] = source[4];
    result[5] = source[5];
    result[6] = source[6];
    result[7] = source[7];
    result[8] = source[8];
    result[10] = source[10];
    result[11] = source[11];
    result[12] = source[12];

    return result;
}

HOOK(0x004C37F0, S_004bdcd0_AppendRecord, "auto");
