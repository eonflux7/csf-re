#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destructor for C_0082fe88 (derives from C_0082d410). Releases the two owned
// blocks at +0x14 and +0x04, resets the vtable to the base, and frees the object
// itself when bit 0 of deleteFlag is set. Returns the object address.
static uint32_t* __fastcall C_0082fe88_Destroy(C_0082fe88* self, void* /*edx*/, char deleteFlag) {
    CSF_AT(void**, self, 0) = CSF_VTBL(0x0082FE88);

    if (self->field_0014) {
        Mem_Free(reinterpret_cast<void*>(self->field_0014));
        self->field_0014 = 0;
    }
    if (self->field_0004) {
        Mem_Free(reinterpret_cast<void*>(self->field_0004));
        self->field_0004 = 0;
    }

    CSF_AT(void**, self, 0) = CSF_VTBL(0x0082D410);

    if ((deleteFlag & 1) != 0) {
        Mem_Free(static_cast<void*>(self));
    }

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x425E40, C_0082fe88_Destroy, "auto");
