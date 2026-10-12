#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Destroys the combat task owned by the object (base cleanup run before release).
CSF_FUNC(int, __fastcall, AiCore_DestroyCombatTask, 0x004B9560, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for TCombate: runs the combat-task cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer.
static char* __fastcall TCombate_DeletingDestroy(void* self, void* /*edx*/, char flags)
{
    AiCore_DestroyCombatTask(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return static_cast<char*>(self);
}

HOOK(0x4B96B0, TCombate_DeletingDestroy, "auto");
