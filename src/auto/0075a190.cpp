#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Resets the vtable to C_0086c12c, frees the buffer held in field_0004 if present,
// clears the pointer, then restores the base-class vtable (C_0082d410).
// Returns the Mem_Free result when a buffer was freed, otherwise the original field value (0).
static int __fastcall C_0086c12c_ReleaseOwnedBuffer(C_0086c12c* self, void* /*edx*/) {
    self->vtbl = CSF_VTBL(0x0086C12C);
    int result = (int)self->field_0004;
    if (result) {
        result = Mem_Free(reinterpret_cast<void*>(self->field_0004));
        self->field_0004 = 0;
    }
    self->vtbl = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x0075A190, C_0086c12c_ReleaseOwnedBuffer, "auto");
