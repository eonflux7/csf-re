#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: releases one child entry (C_0082daf4). Bound by address, __fastcall with (self, edx).
CSF_FUNC(int, __fastcall, sub_40B770, 0x0040B770, (void* self, void* edx));

// Walks the entry array at this+0x1c, calling the per-entry routine for each of the
// `count` entries at this+0x24. The count and the array pointer are re-read on every
// iteration, as the original does. Returns the final count.
static uint32_t __fastcall ShutdownAllEntries(uint32_t* self, void* /*edx*/)
{
    for (uint32_t i = 0; i < CSF_AT(uint32_t, self, 0x24); ++i)
    {
        auto* entries = reinterpret_cast<C_0082daf4**>(CSF_AT(uint32_t, self, 0x1c));
        sub_40B770(entries[i], nullptr);
    }
    return CSF_AT(uint32_t, self, 0x24);
}

HOOK(0x408590, ShutdownAllEntries, "auto");
