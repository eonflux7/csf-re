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

CSF_FUNC(unsigned int, __fastcall, sub_4017A0, 0x004017A0, (void* self, void* edx, int a2));

// Appends a 20-byte record (5 dwords, copies dwords 0,2,3,4 of the source) to the
// growable array at self->field_0000, growing it first when full. Returns the new slot.
static uint32_t* __fastcall Entries_AppendRecord(S_004017a0* self, void* /*edx*/, uint32_t* a2, int a3)
{
    if (self->field_0008 >= self->field_0004) {
        int growArg = a3;
        if (!a3)
            growArg = static_cast<int>(self->field_000c);
        sub_4017A0(self, nullptr, growArg);
    }

    uint32_t index = self->field_0008;
    uint32_t* result = reinterpret_cast<uint32_t*>(self->field_0000 + 20 * index);
    self->field_0008 = index + 1;

    result[0] = a2[0];
    result[2] = a2[2];
    result[3] = a2[3];
    result[4] = a2[4];
    return result;
}

HOOK(0x00401C80, Entries_AppendRecord, "auto");
