#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Heap callees bound by address. Mem_FreeDebug releases the entry buffer,
// Mem_Free releases the object itself.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Resets the object's three buffer fields (releasing the buffer at field_0008
// first, if set) and, when bit 0 of freeFlag is set, frees the object itself.
// Returns the object's vtable slot address, which is the object itself.
static void** __fastcall C_0086a3c0_Destroy(C_0086a3c0* self, void* /*edx*/, char freeFlag) {
    self->vtbl = CSF_VTBL(0x0086A3C0);

    if (self->field_0008) {
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0008));
    }

    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;

    if ((freeFlag & 1) != 0) {
        Mem_Free(self);
    }

    return reinterpret_cast<void**>(self);
}

HOOK(0x007568D0, C_0086a3c0_Destroy, "auto");
