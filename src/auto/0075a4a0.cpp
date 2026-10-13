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

// Returns the index of the first 68-byte entry in this->field_0004 (S_0075b5e0 list)
// whose leading dword equals searchKey, or -1 when no entry matches.
static int __fastcall C_0086c1e0_FindEntryIndex(C_0086c1e0* self, void* /*edx*/, int searchKey)
{
    // this->field_0004 holds a pointer to the entry list (S_0075b5e0).
    void* entryList = reinterpret_cast<void*>(self->field_0004);
    uint32_t entryIndex = 0;

    for (int byteOffset = 0; ; byteOffset += 68)
    {
        uint32_t entriesBegin = CSF_AT(uint32_t, entryList, 0x4);
        if (entriesBegin == 0)
            break;

        // Entry count = (end - begin) / 68, signed division; compared unsigned against the index.
        uint32_t entriesEnd = CSF_AT(uint32_t, entryList, 0x8);
        int byteSpan = static_cast<int>(entriesEnd - entriesBegin);
        int entryCount = byteSpan / 68;
        if (entryIndex >= static_cast<uint32_t>(entryCount))
            break;

        uint8_t* entryPtr = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(entriesBegin)) + byteOffset;
        if (searchKey == *reinterpret_cast<int*>(entryPtr))
            return static_cast<int>(entryIndex);

        ++entryIndex;
    }

    return -1;
}

HOOK(0x0075A4A0, C_0086c1e0_FindEntryIndex, "auto");
