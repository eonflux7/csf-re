#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Copies a waypoint table header from src into self. When src holds entries, allocates
// a table of 8-byte slots initialised to -1 pairs and copies (8 * src[2]) bytes from src[0]
// into it. Stores the table pointer at self[0], then copies src[2], src[1], src[3] into
// self[2], self[1], self[3] (dwords 8, 4, 0xC).
static void* __fastcall WaypointGraph_CloneAndInitTable(void* self, void* /*edx*/, void* src)
{
    const uint32_t entryCount = CSF_AT(uint32_t, src, 4);

    if (entryCount != 0)
    {
        void* table = Mem_Alloc(8 * static_cast<std::size_t>(entryCount));
        if (table != nullptr)
        {
            uint8_t* cursor = static_cast<uint8_t*>(table);
            for (uint32_t i = 0; i < entryCount; ++i)
            {
                CSF_AT(int32_t, cursor, 0) = -1;
                CSF_AT(int32_t, cursor, 4) = -1;
                cursor += 8;
            }
        }
        else
        {
            table = nullptr;
        }

        CSF_AT(void*, self, 0) = table;
        std::memcpy(table, CSF_AT(void*, src, 0),
                    8 * static_cast<std::size_t>(CSF_AT(uint32_t, src, 8)));
    }
    else
    {
        CSF_AT(void*, self, 0) = nullptr;
    }

    CSF_AT(uint32_t, self, 8) = CSF_AT(uint32_t, src, 8);
    CSF_AT(uint32_t, self, 4) = CSF_AT(uint32_t, src, 4);
    CSF_AT(uint32_t, self, 0xC) = CSF_AT(uint32_t, src, 0xC);
    return self;
}

HOOK(0x75BF70, WaypointGraph_CloneAndInitTable, "auto");
