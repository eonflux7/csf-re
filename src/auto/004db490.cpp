#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Scalar-deleting-style teardown for C_0082d410: stores the embedded-interface data
// slot at +0x4, then the vtable at +0x4 and +0x0, and frees the block when bit 0 of
// the flags argument is set. Returns the object pointer (eax = this).
static void* __fastcall C_0082d410_ScalarDestroy(C_0082d410* self, void* /*edx*/, char releaseFlags)
{
    CSF_AT(void*, self, 0x4) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0083DE40));
    CSF_AT(void*, self, 0x4) = CSF_VTBL(0x0082D410);
    *(void***)self = CSF_VTBL(0x0082D410);

    if ((releaseFlags & 1) != 0)
        CoreMemory_Free(self);

    return self;
}

HOOK(0x004DB490, C_0082d410_ScalarDestroy, "auto");
