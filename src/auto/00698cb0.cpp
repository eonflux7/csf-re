#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __stdcall, SortedTable_NormalizeAlnumChar, 0x00698C50, (uint16_t* a1));

static int16_t __fastcall ReadWordFromCursor(S_00698c10* self, void* /*edx*/, uint8_t* okFlag)
{
    uint32_t cursor = self->field_0018;
    uint32_t nextCursor = cursor + 2;
    bool readPastEnd = false;
    uint16_t word;

    if (nextCursor <= self->field_001c)
    {
        word = *reinterpret_cast<const uint16_t*>(cursor);
        self->field_0018 = nextCursor;
    }
    else
    {
        word = *reinterpret_cast<const uint16_t*>(cursor);
        readPastEnd = true;
    }

    uint32_t value = word;
    if (readPastEnd || !SortedTable_NormalizeAlnumChar(reinterpret_cast<uint16_t*>(&value)))
    {
        *okFlag = 0;
        return static_cast<int16_t>(value);
    }

    *okFlag = 1;
    return static_cast<int16_t>(value);
}

HOOK(0x00698CB0, ReadWordFromCursor, "core_sorted_table");
