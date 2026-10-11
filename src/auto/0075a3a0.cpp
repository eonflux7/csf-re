#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Grows the record list when it is full (__thiscall, one stack arg, ret 4).
CSF_FUNC(void*, __fastcall, sub_4DE9B0, 0x004DE9B0, (void* self, void* edx, int a2));

// Appends a 12-byte record (3 dwords copied from `record`) to the list embedded at
// self+0x04 (data ptr +0x04, capacity +0x08, count +0x0C, grow argument +0x10).
// Returns record[1].
static int __fastcall AppendThreeDwordRecord(C_0085e7b8* self, void* /*edx*/, const uint32_t* record)
{
    if (self->field_000c >= self->field_0008)
        sub_4DE9B0(&self->field_0004, nullptr, CSF_AT(int, self, 0x10));

    uint32_t index = self->field_000c;
    uint32_t* slot = reinterpret_cast<uint32_t*>(
        static_cast<uintptr_t>(self->field_0004) + static_cast<uintptr_t>(index) * 12u);
    self->field_000c = index + 1;

    slot[0] = record[0];
    uint32_t result = record[1];
    slot[1] = result;
    slot[2] = record[2];
    return static_cast<int>(result);
}

HOOK(0x75A3A0, AppendThreeDwordRecord, "auto");
