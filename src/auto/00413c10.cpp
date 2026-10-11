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

CSF_FUNC(unsigned int, __fastcall, MissionScript_GrowEventRecords, 0x00413570, (void* self, void* edx, int a2));

// Appends one 12-byte record to the S_00413570 array, growing it when full.
// Record layout: +0 unused (filled by caller/other code), +4 and +8 copied from source[+4] and source[+8].
static int __fastcall S_00413570_AppendRecord(S_00413570* self, void* /*edx*/, int a2, int a3)
{
    // The original references this vtable address; it is not written here.
    (void)CSF_VTBL(0x00413570);

    if (self->field_0008 >= self->field_0004)
    {
        int growCount = a3;
        if (!a3)
            growCount = (int)self->field_000c;
        MissionScript_GrowEventRecords(self, nullptr, growCount);
    }

    uint32_t index = self->field_0008;
    uint32_t base = self->field_0000;
    self->field_0008 = index + 1;

    uint32_t recordAddress = base + 12u * index;
    uint8_t* record = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(recordAddress));
    uint8_t* source = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a2));

    CSF_AT(uint32_t, record, 4) = CSF_AT(uint32_t, source, 4);
    CSF_AT(uint32_t, record, 8) = CSF_AT(uint32_t, source, 8);

    return static_cast<int>(recordAddress);
}

HOOK(0x00413C10, S_00413570_AppendRecord, "auto");
