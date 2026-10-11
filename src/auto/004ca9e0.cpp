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

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __stdcall, sub_4C9020, 0x004C9020, (int a1));

// Resets the cover-attack task state on the object linked to a1 and cancels
// the pending animation/action on its sub-object, then clears the follow-up flag.
static int __stdcall ResetCoverAttackState(int a1)
{
    // First lookup: clear the 0x6DC counter on the linked task object (base = linked - 4).
    int linkedValue = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<void*>(static_cast<uintptr_t>(a1)), nullptr);
    uint8_t* taskObject = linkedValue ? reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(linkedValue - 4))
                                      : nullptr;
    CSF_AT(int, taskObject, 0x6DC) = 0;

    // Second lookup: cancel the sub-object's pending action via its vtable slot 140/4 = 35.
    int linkedValueForSub = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<void*>(static_cast<uintptr_t>(a1)), nullptr);
    uint8_t* subOwner = linkedValueForSub ? reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(linkedValueForSub - 4))
                                          : nullptr;
    void* subObject = CSF_AT(void*, subOwner, 0x2C);
    ((int(__fastcall*)(void*, void*, int, int, int, int))CSF_AT(void**, subObject, 0)[140 / 4])(
        subObject, nullptr, 0, 0, -1, 0);

    sub_4C9020(a1);

    // Third lookup: clear the follow-up flag on the linked object, or the absolute field at 0x6B0.
    int result = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<void*>(static_cast<uintptr_t>(a1)), nullptr);
    if (result)
    {
        CSF_AT(uint8_t, reinterpret_cast<void*>(static_cast<uintptr_t>(result)), 0x6AC) = 0;
        return result;
    }
    // Original writes [eax + 0x6B0] with eax == 0, i.e. a null-based address.
    uint8_t* nullBase = nullptr;
    CSF_AT(uint8_t, nullBase, 0x6B0) = 0;
    return 0;
}

HOOK(0x004CA9E0, ResetCoverAttackState, "auto");
