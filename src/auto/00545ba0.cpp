#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Destructor body of CMenuSaveGame (destroys the screen's widgets/listbox).
CSF_FUNC(void, __fastcall, CMenuSaveGame_Destruct, 0x5455C0,
         (CMenuSaveGame* self, void* edx));

// Deleting destructor wrapper: runs the destructor body, then releases the
// object back to the game heap when bit 0 of the flags argument is set.
static void* __fastcall CMenuSaveGame_Destroy(
    CMenuSaveGame* self, void* /*edx*/, char flags)
{
    CMenuSaveGame_Destruct(self, nullptr);
    if ((flags & 1) != 0)
        game::Mem_Free(self);
    return self;
}

HOOK(0x545BA0, CMenuSaveGame_Destroy, "auto");

