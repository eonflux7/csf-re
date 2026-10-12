#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Game heap free (__cdecl, pops nothing from the caller's frame beyond its own cdecl cleanup).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// C_0082fe88 destructor body: restores C_0082fe88's vtable, frees the blocks held at
// +0x14 and +0x04 (zeroing each), then restores the base class C_0082d410's vtable.
// Returns the value of field_0004 as read before the free (or Mem_Free's result if it was set).
static int __fastcall C_0082fe88_ReleaseOwnedBlocks(C_0082fe88* self, void* /*edx*/)
{
    int result;

    self->vtbl = CSF_VTBL(0x0082FE88);

    if (self->field_0014) {
        Mem_Free(reinterpret_cast<void*>(self->field_0014));
        self->field_0014 = 0;
    }

    result = static_cast<int>(self->field_0004);
    if (result) {
        result = Mem_Free(reinterpret_cast<void*>(self->field_0004));
        self->field_0004 = 0;
    }

    self->vtbl = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x425AE0, C_0082fe88_ReleaseOwnedBlocks, "auto");
