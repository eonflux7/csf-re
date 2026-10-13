#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Tracking-task teardown run before the object is released (sub_4AE700).
CSF_FUNC(int, __fastcall, AiCore_DestroyTrackingTask, 0x004AE700, (void* self, void* edx));

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Deleting destructor for TFncRastrear: runs the tracking-task teardown, then
// frees the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original does.
static void* __fastcall TFncRastrear_Destroy(TFncRastrear* self, void* /*edx*/, char flags)
{
    AiCore_DestroyTrackingTask(self, nullptr);
    if ((flags & 1) != 0)
        CoreMemory_Free(self);
    return self;
}

HOOK(0x4AE8F0, TFncRastrear_Destroy, "auto");
