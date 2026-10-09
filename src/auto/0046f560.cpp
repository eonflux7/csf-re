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

// Per-entry handler: __fastcall, ecx = entry pointer, edx unused.
CSF_FUNC(uint32_t*, __fastcall, MissionScript_ProcessEntry, 0x00471D00, (void* self, void* edx));

// Walks the 12-byte entries between the begin pointer (+0x08) and end pointer (+0x0C)
// and runs MissionScript_ProcessEntry on each. Returns the last handler's result,
// or 0 when there are no entries.
static int __fastcall MissionScript_RunEntryHandlerForEach(void* self, void* /*edx*/)
{
    int result = static_cast<int>(CSF_AT(uint32_t, self, 0x08));

    if (result != 0)
    {
        // Entry count = byte span / 12 (signed division, as in the original).
        const int32_t byteSpan = static_cast<int32_t>(CSF_AT(uint32_t, self, 0x0C) - CSF_AT(uint32_t, self, 0x08));
        result = byteSpan / 12;
    }

    if (result != 0)
    {
        int offset = 0;
        uint32_t remaining = static_cast<uint32_t>(result);
        do
        {
            // The begin pointer is re-read on every iteration, as in the original.
            void* entry = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x08) + static_cast<uint32_t>(offset));
            result = static_cast<int>(reinterpret_cast<uintptr_t>(MissionScript_ProcessEntry(entry, nullptr)));
            offset += 12;
            --remaining;
        } while (remaining != 0);
    }

    return result;
}

HOOK(0x46F560, MissionScript_RunEntryHandlerForEach, "auto");
