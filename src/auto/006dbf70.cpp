#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step for CRenderEngine, run before the object is released (sub_6DBD50).
CSF_FUNC(void, __fastcall, CRenderEngine_BaseCleanup, 0x006DBD50, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for CRenderEngine: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original leaves it in eax.
static void* __fastcall CRenderEngine_DeletingDestructor(CRenderEngine* self, void* /*edx*/, char flags)
{
    CRenderEngine_BaseCleanup(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x006DBF70, CRenderEngine_DeletingDestructor, "auto");
