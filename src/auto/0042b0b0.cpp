#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Game heap deallocator called by the deleting destructor.
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Scalar deleting destructor for CMenuScreen: restores the base vtable and,
// when the low bit of the flags argument is set, frees the object back to the
// game heap. Returns the object pointer (the original returns this in eax).
static uint32_t* __fastcall CMenuScreen_ScalarDeletingDestructor(
    void* self, void* /*edx*/, char flags)
{
    CSF_AT(void**, self, 0) = CSF_VTBL(0x0083042C);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x42B0B0, CMenuScreen_ScalarDeletingDestructor, "auto");
