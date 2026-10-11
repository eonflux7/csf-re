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

// Returns the index of the first 72-byte entry in the table whose leading
// dword equals `value`, or -1 if no entry matches. The table is delimited by
// this->field_0008 (begin) and this->field_000c (end). The entry count is
// computed from the begin pointer captured on entry and the current end pointer.
static int __fastcall C_0086c288_FindIndexOfValue(C_0086c288* self, void* /*edx*/, int value)
{
    // The null check and the size subtraction use the begin pointer captured on entry.
    const uint32_t tableBegin = self->field_0008;
    uint32_t index = 0;
    uint32_t entryOffset = 0;

    while (tableBegin != 0)
    {
        // Entry count is recomputed every iteration from the current end pointer.
        // Signed division by 72, matching the original's imul/sar sequence.
        const int32_t entryCount =
            static_cast<int32_t>(self->field_000c - tableBegin) / 72;

        // Unsigned comparison, matching the original jae.
        if (!(index < static_cast<uint32_t>(entryCount)))
            break;

        // The element base is reloaded from the begin field on every iteration.
        const uint32_t currentBegin = self->field_0008;
        const int entryKey = *reinterpret_cast<int*>(
            static_cast<uintptr_t>(currentBegin + entryOffset));

        if (value == entryKey)
            return static_cast<int>(index);

        ++index;
        entryOffset += 72;
    }

    return -1;
}

HOOK_KEEP_ECX(0x75CE20, C_0086c288_FindIndexOfValue, 1, "bdd_waypoint_graph");
