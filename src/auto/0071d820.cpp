#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Vtable constant written into the object's vptr slot.
CSF_GLOBAL(void*, g_vtbl_C_00864a58, 0x864A58);

// Global game heap deallocator called by the deleting destructor.
CSF_FUNC(void, __cdecl, Mem_Free, 0x691110, (void* ptr));

// Scalar deleting destructor: restores this class' vtable and, when the low
// bit of the flags argument is set, frees the object back to the game heap.
static C_00864a58* __fastcall C_00864a58_ScalarDeletingDestructor(
    C_00864a58* self, void* /*edx*/, unsigned char flags)
{
    self->vtbl = &g_vtbl_C_00864a58;
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x71D820, C_00864a58_ScalarDeletingDestructor, "auto");
