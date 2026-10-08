#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Destructor body of CMenuMissionInfo (destroys the panels / owned members of the menu screen).
CSF_FUNC(void, __fastcall, CMenuMissionInfo_DestroyBody, 0x53EC00,
         (CMenuMissionInfo* self, void* edx));

// Deleting destructor wrapper: runs the destructor body, then frees the object
// through the game heap when the low bit of the flags argument is set.
static void* __fastcall CMenuMissionInfo_Destroy(
    CMenuMissionInfo* self, void* /*edx*/, char flags)
{
    CMenuMissionInfo_DestroyBody(self, nullptr);
    if ((flags & 1) != 0)
        game::Mem_Free(self);
    return self;
}

HOOK(0x540350, CMenuMissionInfo_Destroy, "auto");
