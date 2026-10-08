#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Destructor body of CMenuLanguage (tears down the screen's members/resources).
CSF_FUNC(void*, __fastcall, CMenuLanguage_DestroyBody, 0x5343E0,
         (CMenuLanguage* self, void* edx));

// Deleting destructor wrapper: runs the destructor body, then releases the
// object through the game heap when the low bit of the flags argument is set.
static void* __fastcall CMenuLanguage_Destroy(
    CMenuLanguage* self, void* /*edx*/, char flags)
{
    CMenuLanguage_DestroyBody(self, nullptr);
    if ((flags & 1) != 0)
        game::Mem_Free(self);
    return self;
}

HOOK(0x5345A0, CMenuLanguage_Destroy, "auto");
