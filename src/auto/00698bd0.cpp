#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Reads the next 16-bit entry from a sorted table cursor object.
// Layout (self = ecx): +0x18 = current read pointer (int16_t*), +0x1c = end pointer.
// On success returns the entry and advances the cursor by one entry.
// On running past the end, sets *outPastEnd = 1 and returns the entry at the
// cursor without advancing it.
static int16_t __fastcall SortedTable_ReadNextWord(void* self, void* /*edx*/, uint8_t* outPastEnd)
{
    *outPastEnd = 0;

    int16_t* cursor = CSF_AT(int16_t*, self, 0x18);
    uintptr_t nextCursor = reinterpret_cast<uintptr_t>(cursor) + 2;
    uintptr_t tableEnd = CSF_AT(uint32_t, self, 0x1c);

    if (nextCursor <= tableEnd) {
        int16_t entry = *cursor;
        CSF_AT(uint32_t, self, 0x18) = static_cast<uint32_t>(nextCursor);
        return entry;
    }

    *outPastEnd = 1;
    // The original loads the pointer stored at +0x18, then reads the word it points to.
    int16_t* cursorAtEnd = CSF_AT(int16_t*, self, 0x18);
    return *cursorAtEnd;
}

HOOK_KEEP_ECX(0x698BD0, SortedTable_ReadNextWord, 1, "core_sorted_table");
