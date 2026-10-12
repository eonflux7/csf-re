#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0083b958_Destroy, 0x004D1140, (C_0083b958* self, void* /*edx*/));

// Resets the vtable chain of C_0083ab58 and tail-jumps into the base-class destructor.
static int __fastcall C_0083ab58_ResetVtablesAndDestroyBase(C_0083ab58* self, void* /*edx*/) {
    self->vtbl = CSF_VTBL(0x0083AB58);
    CSF_AT(uint32_t, self, 0x04) = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x0083AB38);
    self->field_0050 = (uint32_t)(uintptr_t)CSF_VTBL(0x0082D410);
    self->field_0044 = (uint32_t)(uintptr_t)CSF_VTBL(0x0082D410);
    return C_0083b958_Destroy(reinterpret_cast<C_0083b958*>(self), nullptr);
}

HOOK(0x004CD720, C_0083ab58_ResetVtablesAndDestroyBase, "auto");
