#include <cstdint>

#include "hook.h"
#include "types/auto_classes.h"

// Adds a delta to the object's accumulator at +0x08, then clamps it to the
// linked value (vtable slot 0x84) when it exceeds that value. Returns the
// linked value that was last read.
static int __fastcall C_0083e160_AddDeltaClampedToLinkedValue(C_0083e160* self, void* /*edx*/, int a2) {
    self->field_0008 += static_cast<uint32_t>(a2);
    int result = self->vGetLinkedValueDC();
    if (static_cast<int32_t>(self->field_0008) > result) {
        result = self->vGetLinkedValueDC();
        self->field_0008 = static_cast<uint32_t>(result);
    }
    return result;
}

HOOK(0x4E11D0, C_0083e160_AddDeltaClampedToLinkedValue, "auto");
