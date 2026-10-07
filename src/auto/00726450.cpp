#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Vtable constant written into the object's vptr slot.
CSF_GLOBAL(void*, g_vtbl_C_008655c0, 0x8655C0);

// Global game heap deallocator called by the deleting destructor.
CSF_FUNC(int, __cdecl, Mem_Free, 0x691110, (void* ptr));

// Scalar deleting destructor: restores this class' vtable and, when the low
// bit of the flags argument is set, frees the object back to the game heap.
static C_008655c0* __fastcall C_008655c0_ScalarDeletingDestructor(
    C_008655c0* self, void* /*edx*/, unsigned char flags)
{
    self->vtbl = &g_vtbl_C_008655c0;
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x726450, C_008655c0_ScalarDeletingDestructor, "auto");
