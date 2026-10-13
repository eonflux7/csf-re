#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns this->field_000c + 4 * index (32-bit wrap-around, as in the original).
static int __fastcall C_0086243c_ComputeIndexedOffset(C_0086243c* self, void* /*edx*/, int index) {
    return static_cast<int>(self->field_000c + static_cast<uint32_t>(index) * 4u);
}

HOOK(0x6F1640, C_0086243c_ComputeIndexedOffset, "auto");
