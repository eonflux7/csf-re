#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns 1 when every bit set in a2 is also set in this->field_0008
// (a2 is a subset of the object's flag mask), otherwise 0.
static int __fastcall C_008656d0_IsMaskSubset(C_008656d0* self, void* /*edx*/, int mask) {
    return mask == (mask & static_cast<int>(self->field_0008));
}

HOOK(0x6C5780, C_008656d0_IsMaskSubset, "auto");
