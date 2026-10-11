#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_513B10).
CSF_FUNC(int, __fastcall, sub_513B10, 0x00513B10, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_008408c0: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original does (mov eax, esi).
static int* __fastcall C_008408c0_Destroy(C_008408c0* self, void* /*edx*/, char flags)
{
    sub_513B10(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<int*>(self);
}

HOOK(0x513B70, C_008408c0_Destroy, "auto");
