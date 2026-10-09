#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

// Callee: 0x455530, __thiscall with 4 stack args (ret 12).
CSF_FUNC(int, __fastcall, RefreshEntryRange, 0x00455530,
         (void* self, void* edx, int a1, unsigned int a3, int arg8));

// Given a value a3 and a sub-entry index computed from the table's
// begin/end pointers, refreshes the range and writes the address of the
// matching 48-byte entry to *out. Returns out.
static uint32_t* __fastcall S_00455ed0_ResolveEntryAddress(S_00455ed0* self, void* /*edx*/,
                                                            uint32_t* out, int value, int extra)
{
    uint32_t entryBegin = self->field_0004;
    int entryIndex = 0;

    if (entryBegin != 0) {
        int entryCount = (int)(self->field_0008 - entryBegin) / 48;
        if (entryCount != 0) {
            entryIndex = (int)((uint32_t)value - entryBegin) / 48;
        }
    }

    RefreshEntryRange(self, nullptr, value, 1u, extra);

    *out = self->field_0004 + 48u * (uint32_t)entryIndex;
    return out;
}

HOOK(0x00455ED0, S_00455ed0_ResolveEntryAddress, "auto");
