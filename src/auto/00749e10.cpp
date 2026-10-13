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

// Walks 16-byte slot records starting at this+0x50 (step 0x10 per iteration).
// Each record's dwords are tested against -1 at offsets -4, 0, +4, +8 relative
// to the cursor. The first index in the walk whose slot is free is returned;
// returns -1 if nothing matched.
static int __fastcall S_00749d70_FindFreeSlotIndex(S_00749d70* self, void* /*edx*/)
{
    int result = -1;
    int index = 2;
    uint8_t* cursor = reinterpret_cast<uint8_t*>(self) + 0x50;

    do
    {
        if (result != -1)
            break;

        // Candidate index-2 (checked against the dword at cursor-4).
        if (index - 2 < 0 || index - 2 >= 16 || CSF_AT(int, cursor, -4) == -1)
        {
            result = index - 2;
            if (index != 1)
                break;
        }

        // Candidate index-1 (checked against the dword at cursor+0).
        if (index - 1 < 0 || index - 1 >= 16 || CSF_AT(int, cursor, 0) == -1)
            return index - 1;

        // Candidate index (checked against the dword at cursor+4).
        if (index < 0 || index >= 16 || CSF_AT(int, cursor, 4) == -1)
            return index;

        // Candidate index+1 (checked against the dword at cursor+8).
        if (index + 1 < 0 || index + 1 >= 16 || CSF_AT(int, cursor, 8) == -1)
            result = index + 1;

        index += 4;
        cursor += 0x10;
    } while (index - 2 < 16);

    return result;
}

HOOK(0x00749E10, S_00749d70_FindFreeSlotIndex, "auto");
