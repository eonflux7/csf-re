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

// Grows the record buffer of an S_00768e80 when it is full (original 0x768E80).
CSF_FUNC(char, __fastcall, GrowRecordBuffer, 0x00768E80, (void* self, void* edx, int a2));

// The original never stores a vtable here; the reference is kept because the
// packet lists it for this function. The value is never written or read.
[[maybe_unused]] static void* const s_recordBufferVtableRef = CSF_VTBL(0x00768E80);

// Appends a 12-byte record (three dwords) copied from `source` to the buffer.
// If the buffer is full, it grows first, using `growCount` or the stored grow step
// when `growCount` is zero. Returns a pointer to the new record.
static uint32_t* __fastcall S_00768e80_AppendRecord(S_00768e80* self, void* /*edx*/,
                                                    uint32_t* source, int growCount)
{
    if (self->field_0008 >= self->field_0004)
    {
        int growArg = growCount;
        if (growArg == 0)
            growArg = static_cast<int>(self->field_000c);
        GrowRecordBuffer(self, nullptr, growArg);
    }

    uint32_t index = self->field_0008;
    uint32_t bufferBase = self->field_0000;
    self->field_0008 = index + 1;

    uint32_t* record = reinterpret_cast<uint32_t*>(
        static_cast<uintptr_t>(bufferBase) + 12u * static_cast<uintptr_t>(index));

    record[0] = source[0];
    record[1] = source[1];
    record[2] = source[2];
    return record;
}

HOOK(0x769180, S_00768e80_AppendRecord, "auto");
