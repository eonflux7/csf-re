#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Scalar-deleting-style teardown: resets the object's vtable slots, then frees the
// block when bit 0 of the flags argument is set. Returns the object pointer (eax = this).
static void* __fastcall C_00830438_ScalarDestroy(C_00830438* self, void* /*edx*/, char releaseFlags)
{
    CSF_AT(void*, self, 0xC) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x00831450));
    CSF_AT(void*, self, 0xC) = CSF_VTBL(0x0082D410);
    *(void***)self = CSF_VTBL(0x00830438);

    if ((releaseFlags & 1) != 0)
        Mem_Free(self);

    return self;
}

HOOK(0x00468660, C_00830438_ScalarDestroy, "auto");
