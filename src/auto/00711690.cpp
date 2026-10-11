#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_711500).
CSF_FUNC(int, __fastcall, sub_711500, 0x00711500, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_00863cd4: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer as int*, matching the original's (int*)this.
static int* __fastcall C_00863cd4_Destroy(C_00863cd4* self, void* /*edx*/, char flags)
{
    sub_711500(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<int*>(self);
}

HOOK(0x711690, C_00863cd4_Destroy, "auto");
