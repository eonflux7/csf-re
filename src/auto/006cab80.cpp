#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(int, __fastcall, sub_6CA970, 0x006CA970, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_6F5750, 0x006F5750, (void* self, void* edx));

// Ensures the object has its backing handle (field_0010), creating it on demand,
// then returns the result of sub_6F5750 applied to that handle.
static int __fastcall C_00860bb8_EnsureAndGetWorldHandle(C_00860bb8* self, void* /*edx*/) {
    if (!self->field_0010) {
        sub_6CA970(self, nullptr);
    }
    // Reload the handle: sub_6CA970 may have populated it.
    return sub_6F5750(reinterpret_cast<void*>(self->field_0010), nullptr);
}

HOOK(0x006CAB80, C_00860bb8_EnsureAndGetWorldHandle, "auto");
