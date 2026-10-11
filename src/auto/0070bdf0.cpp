#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Teardown for C_0082d410: first points the +0x4 slot at the data item 0x863780,
// then reinstalls the class vtable there and at +0x0. Frees the block when bit 0
// of the flags argument is set. Returns the object pointer (eax = this).
static void* __fastcall C_0082d410_DestroyAndMaybeFree(C_0082d410* self, void* /*edx*/, char releaseFlags)
{
    CSF_AT(void*, self, 0x4) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x00863780));
    CSF_AT(void*, self, 0x4) = CSF_VTBL(0x0082D410);
    *(void***)self = CSF_VTBL(0x0082D410);

    if ((releaseFlags & 1) != 0)
        Mem_Free(self);

    return self;
}

HOOK(0x0070BDF0, C_0082d410_DestroyAndMaybeFree, "auto");
