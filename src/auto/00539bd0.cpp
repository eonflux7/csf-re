#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Destructor body of CMenuLoadGame (releases owned members / resources).
CSF_FUNC(void, __fastcall, CMenuLoadGame_DestroyBody, 0x539B20,
         (CMenuLoadGame* self, void* edx));

// Deleting destructor wrapper: runs the destructor body, then frees the object
// through the game heap when the low bit of the flags argument is set.
static CMenuLoadGame* __fastcall CMenuLoadGame_Destroy(
    CMenuLoadGame* self, void* /*edx*/, char flags)
{
    CMenuLoadGame_DestroyBody(self, nullptr);
    if ((flags & 1) != 0)
        game::Mem_Free(self);
    return self;
}

HOOK(0x539BD0, CMenuLoadGame_Destroy, "auto");
