#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Searches the table of 12-byte entries [begin, end) for an entry whose
// second dword (at +4) equals searchValue. Returns the entry index or -1.
static int __fastcall SoundAreas_FindEntryIndex(S_00771b80* self, void* /*edx*/, int searchValue)
{
    uint32_t tableBegin = self->field_0004;
    unsigned int entryCount = 0;
    if (tableBegin != 0) {
        entryCount = static_cast<unsigned int>(static_cast<int>(self->field_0008 - tableBegin) / 12);
    }

    int index = 0;
    if (entryCount == 0) {
        return -1;
    }

    uint32_t* key = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(tableBegin) + 4);
    for (; static_cast<int>(*key) != searchValue; key += 3) {
        ++index;
        if (static_cast<unsigned int>(index) >= entryCount) {
            return -1;
        }
    }
    return index;
}

HOOK(0x00771B80, SoundAreas_FindEntryIndex, "auto");
