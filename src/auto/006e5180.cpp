#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Teardown for the gfx alt-instance object: points the slot at +0x4 first at its data
// block, then at its interface vtable, sets the primary vtable, and frees the block
// when bit 0 of the flags argument is set. Returns the object pointer (eax = this).
static void* __fastcall C_00861bd8_ScalarDestroy(C_00861bd8* self, void* /*edx*/, char releaseFlags)
{
    CSF_AT(void*, self, 0x4) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x00861BBC));
    CSF_AT(void*, self, 0x4) = CSF_VTBL(0x0082D410);
    *(void***)self = CSF_VTBL(0x00861B9C);

    if ((releaseFlags & 1) != 0)
        Mem_Free(self);

    return reinterpret_cast<void*>(self);
}

HOOK(0x006E5180, C_00861bd8_ScalarDestroy, "auto");
