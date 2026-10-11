#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Teardown for C_0082d410: points the secondary slot at the data entry, then the
// class vtable, and frees the block when bit 0 of the flags argument is set.
// Returns the object pointer (eax = this).
static void* __fastcall C_0082d410_ScalarDestroy(C_0082d410* self, void* /*edx*/, char releaseFlags)
{
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x0082D7C8));
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
    self->vtbl = CSF_VTBL(0x0082D410);

    if ((releaseFlags & 1) != 0)
        Mem_Free(self);

    return self;
}

HOOK(0x00409260, C_0082d410_ScalarDestroy, "auto");
