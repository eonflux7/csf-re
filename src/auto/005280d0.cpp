#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Scalar deleting destructor: resets both vtable slots to 0x82D410, then frees the
// block when bit 0 of the flags argument is set. Returns the object pointer.
static void* __fastcall C_008411a0_ScalarDeletingDestructor(void* self, void* /*edx*/, char deleteFlags)
{
    CSF_AT(void*, self, 0x28) = CSF_VTBL(0x0082D410);
    *static_cast<void**>(self) = CSF_VTBL(0x0082D410);
    if ((deleteFlags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x005280D0, C_008411a0_ScalarDeletingDestructor, "auto");
