#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Releases the sub-objects owned by C_00869124 (sub_753AC0). Return value is not used.
CSF_FUNC(int, __fastcall, C_00869124_ReleaseMembers, 0x00753AC0, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_00869124: runs the member cleanup, then frees the
// object through the game heap when the low bit of the flags argument is set.
// Returns the object's vtable slot address, which is the object itself (vtable at offset 0).
static void** __fastcall C_00869124_Destroy(C_00869124* self, void* /*edx*/, char flags)
{
    C_00869124_ReleaseMembers(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<void**>(self);
}

HOOK(0x753B10, C_00869124_Destroy, "auto");
