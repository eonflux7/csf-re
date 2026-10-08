#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x0052E440 - destructor body of CMenuScenarioSelect (releases members)
CSF_FUNC(int, __fastcall, CMenuScenarioSelect_DestroyBody, 0x52E440,
         (CMenuScenarioSelect* self, void* edx));

// 0x0052E4F0 - deleting-destructor thunk: run the destructor body, then free
// the object when the low bit of the flags argument is set.
static void* __fastcall CMenuScenarioSelect_Destroy(
    CMenuScenarioSelect* self, void* /*edx*/, char flags)
{
    CMenuScenarioSelect_DestroyBody(self, nullptr);
    if ((flags & 1) != 0)
        game::Mem_Free(self);
    return self;
}

HOOK(0x52E4F0, CMenuScenarioSelect_Destroy, "auto");
