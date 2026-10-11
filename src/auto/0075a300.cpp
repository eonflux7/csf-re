#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destructor body for C_0086c12c: resets the owned buffer, then frees
// the object itself when bit 0 of the flags argument is set.
static void** __fastcall C_0086c12c_Destroy(C_0086c12c* self, void* /*edx*/, char a2) {
    CSF_AT(void**, self, 0) = CSF_VTBL(0x0086C12C);
    if (self->field_0004) {
        Mem_Free(reinterpret_cast<void*>(self->field_0004));
        self->field_0004 = 0;
    }
    CSF_AT(void**, self, 0) = CSF_VTBL(0x0082D410);
    if ((a2 & 1) != 0) {
        Mem_Free(self);
    }
    return reinterpret_cast<void**>(self);
}

HOOK(0x0075A300, C_0086c12c_Destroy, "auto");
