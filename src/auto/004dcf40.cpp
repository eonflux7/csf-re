#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S_004dca10_GrowStorage, 0x004DCA10, (void* self, void* edx, int a2));

// Appends one 28-byte record (copies dwords 0, 2..6 of the source) to the
// buffer at field_0000, growing it first when count >= capacity.
static uint32_t* __fastcall S_004dca10_AppendRecord(S_004dca10* self, void* /*edx*/, uint32_t* record, int growHint)
{
    if (self->field_0008 >= self->field_0004)
    {
        int growArg = growHint;
        if (growArg == 0)
            growArg = static_cast<int>(self->field_000c);
        S_004dca10_GrowStorage(self, nullptr, growArg);
    }

    uint32_t index = self->field_0008;
    uint32_t* result = reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(self->field_0000) + index * 28);
    self->field_0008 = index + 1;

    result[0] = record[0];
    result[2] = record[2];
    result[3] = record[3];
    result[4] = record[4];
    result[5] = record[5];
    result[6] = record[6];
    return result;
}

HOOK(0x004DCF40, S_004dca10_AppendRecord, "auto");
