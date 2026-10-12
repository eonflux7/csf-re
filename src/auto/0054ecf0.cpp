#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Appends one 20-byte record (5 dwords copied from a2) to a 100-slot ring buffer
// stored at this+0x10, advances the write index with wrap-around at 100, and
// returns the running record counter after incrementing it.
static int __fastcall S_0054ecf0_PushRecord(S_0054ecf0* self, void* /*edx*/, uint32_t* a2) {
    const uint32_t writeIndex = self->field_108c;
    const uint32_t slotOffset = 0x10 + 20 * writeIndex;

    for (int i = 0; i < 5; ++i) {
        CSF_AT(uint32_t, self, slotOffset + 4 * i) = a2[i];
    }

    const uint32_t nextIndex = writeIndex + 1;
    self->field_108c = nextIndex;
    if (nextIndex == 100) {
        self->field_108c = 0;
    }

    const uint32_t previousCount = self->field_1090;
    const int result = static_cast<int>(previousCount + 1);
    self->field_1090 = static_cast<uint32_t>(result);
    return result;
}

HOOK(0x0054ECF0, S_0054ecf0_PushRecord, "auto");
