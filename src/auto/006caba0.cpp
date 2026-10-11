#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(int, __fastcall, sub_6CA970, 0x006CA970, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_6F5790, 0x006F5790, (void* self, void* edx));

// Ensures the object has its backing handle (field_0010), creating it on demand,
// then returns the result of sub_6F5790 applied to that handle (tail call in the original).
static int __fastcall C_00860bb8_EnsureHandleAndForward(C_00860bb8* self, void* /*edx*/) {
    if (!self->field_0010) {
        sub_6CA970(self, nullptr);
    }
    // Reload the handle: sub_6CA970 may have populated it.
    return sub_6F5790(reinterpret_cast<void*>(self->field_0010), nullptr);
}

HOOK(0x006CABA0, C_00860bb8_EnsureHandleAndForward, "auto");
