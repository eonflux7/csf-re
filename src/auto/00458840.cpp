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

// Looks up an entry by its key in the table of 20-byte records held at
// this->field_002c .. this->field_0030. Each record is 5 dwords: key, then 4 values.
// On a match, copies the 4 values to outValues and returns true.
static bool __fastcall StatusIconTable_LookupEntry(S_00458840* self, void* /*edx*/, int key, uint32_t* outValues)
{
    if (key == -1)
        return false;

    uint32_t entryCount = self->field_002c;
    if (entryCount)
        entryCount = (uint32_t)((int)(self->field_0030 - self->field_002c) / 20);

    if (!entryCount)
        return false;

    uint32_t* tableBase = reinterpret_cast<uint32_t*>(self->field_002c);
    uint32_t index = 0;
    uint32_t* entry = tableBase;
    while (key != (int)*entry)
    {
        if (++index >= entryCount)
            return false;
        entry += 5;
    }

    if (index >= entryCount)
        return false;

    outValues[0] = tableBase[index * 5 + 1];
    outValues[1] = tableBase[index * 5 + 2];
    outValues[2] = tableBase[index * 5 + 3];
    outValues[3] = tableBase[index * 5 + 4];
    return true;
}

HOOK(0x00458840, StatusIconTable_LookupEntry, "auto");
