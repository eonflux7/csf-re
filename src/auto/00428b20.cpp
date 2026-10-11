#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Initializes a C_00830030 object: installs its vtable, then fills 93 consecutive
// 20-byte slot records starting at offset +4. Each record: {-1, -1, 0, 1.0f, 0}.
// Returns the object pointer (the original leaves it in eax).
static C_00830030* __fastcall C_00830030_InitSlotTable(C_00830030* self, void* /*edx*/) {
    self->vtbl = CSF_VTBL(0x830030);

    uint8_t* cursor = reinterpret_cast<uint8_t*>(self) + 4;
    for (int remaining = 93; remaining != 0; --remaining) {
        CSF_AT(int32_t, cursor, 0x0) = -1;
        CSF_AT(int32_t, cursor, 0x4) = -1;
        CSF_AT(int32_t, cursor, 0x8) = 0;
        CSF_AT(uint32_t, cursor, 0xC) = 0x3F800000u;  // 1.0f
        CSF_AT(int32_t, cursor, 0x10) = 0;
        cursor += 0x14;
    }

    return self;
}

HOOK(0x428B20, C_00830030_InitSlotTable, "auto");
