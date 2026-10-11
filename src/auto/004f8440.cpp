#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Grows the record buffer of S_004f7d10 (called only when the buffer is full).
CSF_FUNC(uint32_t*, __fastcall, GrowRecordBuffer, 0x004F7D10, (void* self, void* edx, int a2));

// Appends a 48-byte record copied from `source` to the buffer and returns a pointer to it.
static uint32_t* __fastcall AppendRecordFromSource(S_004f7d10* self, void* /*edx*/, uint32_t* source, int capacityHint)
{
    if (self->field_0008 >= self->field_0004)
    {
        int growHint = capacityHint;
        if (growHint == 0)
            growHint = static_cast<int>(self->field_000c);
        GrowRecordBuffer(self, nullptr, growHint);
    }

    uint32_t index = self->field_0008;
    uint32_t* record = reinterpret_cast<uint32_t*>(self->field_0000 + 48 * index);
    self->field_0008 = index + 1;

    record[0] = source[0];
    record[2] = source[2];
    record[3] = source[3];
    record[5] = source[5];
    record[6] = source[6];
    record[7] = source[7];
    record[8] = source[8];
    record[9] = source[9];
    record[10] = source[10];
    record[11] = source[11];

    return record;
}

HOOK(0x004F8440, AppendRecordFromSource, "auto");
