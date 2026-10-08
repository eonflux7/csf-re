#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Destructor body of CMenuLoading (releases owned members / resources).
CSF_FUNC(void, __fastcall, CMenuLoading_ReleaseMembersAndResetVtable, 0x5388E0,
         (CMenuLoading* self, void* edx));

// Deleting destructor wrapper: runs the destructor body, then frees the object
// through the game heap when the low bit of the flags argument is set.
static CMenuLoading* __fastcall CMenuLoading_Destroy(
    CMenuLoading* self, void* /*edx*/, char flags)
{
    CMenuLoading_ReleaseMembersAndResetVtable(self, nullptr);
    if ((flags & 1) != 0)
        game::Mem_Free(self);
    return self;
}

HOOK(0x538D30, CMenuLoading_Destroy, "auto");
