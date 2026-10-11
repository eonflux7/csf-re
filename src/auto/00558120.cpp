#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Game heap deallocator called by the deleting destructor.
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Scalar deleting destructor for C_008446cc: restores this class' vtable and,
// when the low bit of the flags argument is set, frees the object back to the
// game heap. Returns the object pointer (the original returns &this->vtbl in eax).
static int* __fastcall C_008446cc_ScalarDeletingDestructor(
    void* self, void* /*edx*/, char flags)
{
    CSF_AT(void**, self, 0) = CSF_VTBL(0x008446CC);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<int*>(self);
}

HOOK(0x558120, C_008446cc_ScalarDeletingDestructor, "auto");
