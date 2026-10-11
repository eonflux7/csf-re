#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Game heap allocator (same address as the original's call target).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Copies a waypoint table (src) into a freshly allocated table owned by self.
// Layout of a waypoint table as used here:
//   +0x0  entry array pointer (12-byte entries)
//   +0x4  entry capacity count (used for allocation and initialization)
//   +0x8  element count (used for the copy size)
//   +0xC  extra value copied through unchanged
// Returns self (the destination), as the original does.
static void* __fastcall WaypointGraph_CopyFrom(void* self, void* /*edx*/, void* src)
{
    const uint32_t entryCount = CSF_AT(uint32_t, src, 0x4);

    if (entryCount != 0)
    {
        void* entryBuffer = Mem_Alloc(static_cast<std::size_t>(12u * entryCount));

        if (entryBuffer != nullptr)
        {
            // Every entry starts as (-1, -1, -1).
            int32_t* entry = static_cast<int32_t*>(entryBuffer);
            for (uint32_t i = 0; i < entryCount; ++i)
            {
                entry[0] = -1;
                entry[1] = -1;
                entry[2] = -1;
                entry += 3;
            }
            CSF_AT(void*, self, 0x0) = entryBuffer;
        }
        else
        {
            CSF_AT(void*, self, 0x0) = nullptr;
        }

        // Copies the source's element count of entries, as the original does.
        std::memcpy(CSF_AT(void*, self, 0x0),
                    CSF_AT(const void*, src, 0x0),
                    static_cast<std::size_t>(12u * CSF_AT(uint32_t, src, 0x8)));
    }
    else
    {
        CSF_AT(void*, self, 0x0) = nullptr;
    }

    CSF_AT(uint32_t, self, 0x8) = CSF_AT(uint32_t, src, 0x8);
    CSF_AT(uint32_t, self, 0x4) = CSF_AT(uint32_t, src, 0x4);
    CSF_AT(uint32_t, self, 0xC) = CSF_AT(uint32_t, src, 0xC);

    return self;
}

HOOK(0x75C000, WaypointGraph_CopyFrom, "auto");
