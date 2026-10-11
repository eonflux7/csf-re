#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Grows the entry buffer of S_0051c1b0 to hold at least newCapacity entries.
CSF_FUNC(unsigned int, __fastcall, S_0051c1b0_GrowEntries, 0x0051C1B0,
         (void* self, void* edx, int newCapacity));

// Appends one 24-byte entry to the buffer and returns a pointer to the new slot.
// Dword [1] of the entry is not copied (the original skips it). If the buffer is
// full, it grows it first, using requestedCapacity or, when that is 0, the
// default at field_000c.
static uint32_t* __fastcall S_0051c1b0_AppendEntry(S_0051c1b0* self, void* /*edx*/,
                                                   uint32_t* source, int requestedCapacity)
{
    if (self->field_0008 >= self->field_0004)
    {
        int capacityArg = requestedCapacity;
        if (capacityArg == 0)
            capacityArg = static_cast<int>(self->field_000c);
        S_0051c1b0_GrowEntries(self, nullptr, capacityArg);
    }

    uint32_t index = self->field_0008;
    uint32_t* result = reinterpret_cast<uint32_t*>(self->field_0000 + 24u * index);
    self->field_0008 = index + 1;

    result[0] = source[0];
    result[2] = source[2];
    result[3] = source[3];
    result[4] = source[4];
    result[5] = source[5];
    return result;
}

HOOK(0x0051CB50, S_0051c1b0_AppendEntry, "auto");
