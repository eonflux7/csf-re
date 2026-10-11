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

// Records in C_00840af8's array are 0x50 bytes apart; each record has a
// uint32 id at +0x48. Returns one past the largest valid id, or 0 if none.
static unsigned int __fastcall C_00840af8_GetNextIndex(C_00840af8* self, void* /*edx*/)
{
    unsigned int nextIndex = 0;

    const uintptr_t arrayBegin = self->field_0008;
    const uintptr_t arrayEnd = self->field_000c;

    uint32_t recordCount = 0;
    if (arrayBegin != 0) {
        // Signed division by 80 (record size) of the byte span.
        const int32_t spanBytes = static_cast<int32_t>(static_cast<uint32_t>(arrayEnd - arrayBegin));
        recordCount = static_cast<uint32_t>(spanBytes / 80);
    }

    if (recordCount != 0) {
        uint8_t* record = reinterpret_cast<uint8_t*>(arrayBegin) + 0x48;
        do {
            const uint32_t recordId = *reinterpret_cast<uint32_t*>(record);
            if (recordId != 0xFFFFFFFFu && recordId >= nextIndex) {
                nextIndex = recordId + 1;
            }
            record += 0x50;
            --recordCount;
        } while (recordCount != 0);
    }

    return nextIndex;
}

HOOK(0x515D50, C_00840af8_GetNextIndex, "auto");
