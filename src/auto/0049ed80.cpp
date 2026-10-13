#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Copies the 3-dword position (+12, +16, +20) of entry `index` from the
// 28-byte-stride table at this->field_0198 into out[0..2]. Returns the third value.
static int __fastcall C_008342e8_CopyControlPosition(C_008342e8* self, void* /*edx*/, int index, uint32_t* out) {
    uint32_t entryOffset = static_cast<uint32_t>(index) * 28u;

    out[0] = *reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(self->field_0198) + entryOffset + 12u);
    out[1] = *reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(self->field_0198) + entryOffset + 16u);

    uint32_t thirdValue = *reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(self->field_0198) + entryOffset + 20u);
    out[2] = thirdValue;
    return static_cast<int>(thirdValue);
}

HOOK(0x0049ED80, C_008342e8_CopyControlPosition, "auto");
