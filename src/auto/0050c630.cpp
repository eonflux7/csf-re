#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Destructor body of CMenuScreenFader (frees owned resources / members).
CSF_FUNC(void, __fastcall, CMenuScreenFader_ReleaseResources, 0x50C370,
         (CMenuScreenFader* self, void* edx));

// Deleting destructor wrapper: runs the destructor body, then frees the object
// through the game heap when the low bit of the flags argument is set.
static void* __fastcall CMenuScreenFader_Destroy(
    CMenuScreenFader* self, void* /*edx*/, char flags)
{
    CMenuScreenFader_ReleaseResources(self, nullptr);
    if ((flags & 1) != 0)
        game::Mem_Free(self);
    return self;
}

HOOK(0x50C630, CMenuScreenFader_Destroy, "auto");
