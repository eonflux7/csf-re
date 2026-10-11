#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Game heap deallocator called by the deleting destructor.
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Scalar deleting destructor for C_00840754: restores this class' vtable and,
// when the low bit of the flags argument is set, frees the object back to the
// game heap. Returns the address of the vtable slot (the object itself) in eax.
static uint32_t* __fastcall C_00840754_ScalarDeletingDestructor(
    void* self, void* /*edx*/, char flags)
{
    CSF_AT(void**, self, 0) = CSF_VTBL(0x00840754);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x5113B0, C_00840754_ScalarDeletingDestructor, "auto");
