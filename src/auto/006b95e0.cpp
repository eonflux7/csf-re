#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Removes the 8-byte entry at `entry` from the array ending at self->field_0008
// by shifting the following entries down, then writes `entry` to *outResult
// and returns outResult.
static uint32_t* __fastcall S_006b9130_RemoveEntryAt(S_006b9130* self, void* /*edx*/,
                                                     uint32_t* outResult, uint32_t* entry) {
    uint32_t* end = reinterpret_cast<uint32_t*>(self->field_0008);
    uint32_t* src = entry + 2;
    if (src != end) {
        uint32_t* dst = entry;
        do {
            dst[0] = src[0];
            dst[1] = src[1];
            src += 2;
            dst += 2;
        } while (src != end);
    }
    self->field_0008 -= 8;
    *outResult = reinterpret_cast<uint32_t>(entry);
    return outResult;
}

HOOK(0x6B95E0, S_006b9130_RemoveEntryAt, "auto");
