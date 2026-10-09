#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies the 0x30-byte record block [first, last) from *source: every record
// first receives source's string (full range) and then the five dwords at
// source+0x1C..+0x2C are copied into the record at +0x1C..+0x2C.
// Returns `last` unchanged.
static C_0082d410* __cdecl Hud_CopyWidgetRange(C_0082d410* first, C_0082d410* last, uint32_t* source)
{
    C_0082d410* result = last;
    uint8_t* cursor = reinterpret_cast<uint8_t*>(first);

    if (first != last)
    {
        do
        {
            AssignStringSubstring(cursor, nullptr, source, 0, 0xFFFFFFFFu);

            CSF_AT(uint32_t, cursor, 0x1C) = source[7];
            CSF_AT(uint32_t, cursor, 0x20) = source[8];
            CSF_AT(uint32_t, cursor, 0x24) = source[9];
            CSF_AT(uint32_t, cursor, 0x28) = source[10];
            CSF_AT(uint32_t, cursor, 0x2C) = source[11];

            result = last;
            cursor += 0x30;
        } while (cursor != reinterpret_cast<uint8_t*>(last));
    }
    return result;
}

HOOK(0x434420, Hud_CopyWidgetRange, "auto");
