#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Destructor body: performs the object's own cleanup (called with ecx = self).
CSF_FUNC(void, __fastcall, C_00867444_Destroy_73B360, 0x73B360,
         (C_00867444* self, void* edx));

// Deleting destructor: cleanup body, then free the object when flag bit 0 is set.
static void* __fastcall C_00867444_Destroy(
    C_00867444* self, void* /*edx*/, char flags)
{
    C_00867444_Destroy_73B360(self, nullptr);
    if ((flags & 1) != 0)
        game::Mem_Free(self);
    return self;
}

HOOK(0x73B4D0, C_00867444_Destroy, "auto");
