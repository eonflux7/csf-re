#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Destructor body of CMenuExtras (releases owned menu resources / members).
CSF_FUNC(void, __fastcall, CMenuExtras_DestroyBody, 0x547720,
         (CMenuExtras* self, void* edx));

// Deleting destructor wrapper: runs the destructor body, then frees the object
// through the game heap when the low bit of the flags argument is set.
static void* __fastcall CMenuExtras_Destroy(
    CMenuExtras* self, void* /*edx*/, char flags)
{
    CMenuExtras_DestroyBody(self, nullptr);
    if ((flags & 1) != 0)
        game::Mem_Free(self);
    return self;
}

HOOK(0x547D20, CMenuExtras_Destroy, "auto");
