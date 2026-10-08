#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Body of the destructor: releases the object's resources but does not free the
// raw block.  Result (eax) is unused by the caller.
CSF_FUNC(int, __fastcall, C_00864508_DestroyMembers, 0x71D0D0,
         (C_00864508* self, void* edx));

// Scalar deleting destructor: runs the destructor body and, when the low flag
// bit is set, releases the object back to the game heap.  Always returns `this`.
static void* __fastcall C_00864508_Destroy(
    C_00864508* self, void* /*edx*/, char flags)
{
    C_00864508_DestroyMembers(self, nullptr);
    if ((flags & 1) != 0)
        game::Mem_Free(self);
    return self;
}

HOOK(0x71D7F0, C_00864508_Destroy, "auto");
