#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x0052FE20 - CMenuSceneSelect body destructor (called before the optional free).
CSF_FUNC(int*, __fastcall, CMenuSceneSelect_DestroyBody, 0x52FE20,
         (CMenuSceneSelect* self, void* edx));

// Scoped deleting destructor: tears the object down and, when bit 0 of the
// incoming flags is set, releases the storage through the game heap.
static void* __fastcall CMenuSceneSelect_Destroy(
    CMenuSceneSelect* self, void* /*edx*/, char flags)
{
    CMenuSceneSelect_DestroyBody(self, nullptr);
    if ((flags & 1) != 0)
        game::Mem_Free(self);
    return self;
}

HOOK(0x52FEC0, CMenuSceneSelect_Destroy, "auto");
