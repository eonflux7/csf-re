#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup for CGuiControl, run before the tactical map is released (sub_436F40).
CSF_FUNC(int, __fastcall, CGuiControl_BaseCleanup, 0x00436F40, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for CGuiTacticalMap (C_00830a48 is not declared in auto_classes.h,
// so the object is taken as an untyped pointer): runs the CGuiControl cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original leaves `this` in eax.
static void* __fastcall CGuiTacticalMap_Destroy(void* self, void* /*edx*/, char flags)
{
    CGuiControl_BaseCleanup(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x437A20, CGuiTacticalMap_Destroy, "auto");
