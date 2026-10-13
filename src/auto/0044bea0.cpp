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

// Looks up the record index for a key; returns -1 when no record matches.
CSF_FUNC(int, __fastcall, FindRecordIndex, 0x0044BC20, (void* self, void* edx, int a2));

// Finds the record for `key`, and if found stores `value` at its +0x3C slot.
// Returns the record's address (or -1 when not found).
static int __fastcall StoreValueAtRecordSlot(S_0044bc20* self, void* /*edx*/, int key, int value)
{
    int index = FindRecordIndex(self, nullptr, key);
    int result = index;
    if (index != -1)
    {
        uint8_t* record = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0010))
                        + 100 * index;
        result = static_cast<int>(reinterpret_cast<uintptr_t>(record));
        CSF_AT(int, record, 0x3C) = value;
    }
    return result;
}

HOOK(0x0044BEA0, StoreValueAtRecordSlot, "auto");
