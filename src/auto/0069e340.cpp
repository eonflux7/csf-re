#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Game heap deallocator called by the deleting destructor.
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Scalar deleting destructor for C_0085eb24: restores this class' vtable and,
// when the low bit of the flags argument is set, frees the object back to the
// game heap. Returns the object pointer (the original returns this in eax).
static uint32_t* __fastcall C_0085eb24_ScalarDeletingDestructor(
    void* self, void* /*edx*/, char flags)
{
    CSF_AT(void**, self, 0) = CSF_VTBL(0x0085EB24);
    if ((flags & 1) != 0)
        CoreMemory_Free(self);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x69E340, C_0085eb24_ScalarDeletingDestructor, "auto");
