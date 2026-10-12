#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Linked-object lookup: returns the linked object value, or 0 when none is linked.
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void* edx));
// Applies the task-state update using the linked object's offset plus 0x574.
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));
// Sets the cover-attack state on the cached object provider.
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));
// Returns the cached object provider for this object.
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static int __stdcall Estatico_ApplyLinkedObjectTaskState(C_0085e7b8* a1)
{
    // First pass: base of the linked object (value - 4), or 0 if none.
    int linkedValue = GetLinkedObjectValue_4DB5C0(a1, nullptr);
    int taskBase;
    if (linkedValue)
        taskBase = linkedValue - 4;
    else
        taskBase = 0;
    UnnamedApply_4DC180(a1, nullptr, taskBase + 0x574);

    // Second pass: linked object base again, used for the state offset.
    int linkedValueAgain = GetLinkedObjectValue_4DB5C0(a1, nullptr);
    int stateBase;
    if (linkedValueAgain)
        stateBase = linkedValueAgain - 4;
    else
        stateBase = 0;

    void* cachedProvider = GetCachedObjectProvider(a1, nullptr);
    return CoverAttack_SetState_4DC150(cachedProvider, nullptr, stateBase + 0x584);
}

HOOK(0x4AFE10, Estatico_ApplyLinkedObjectTaskState, "auto");
