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
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, Dispatch_NoLinkedTarget, 0x004DBCB0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

// Dispatches a move task through the linked object: if the linked object's
// sub-object flag at +0x39 is set, dispatch to the "no linked target" path (5);
// otherwise reset the linked task state using the linked field at +0x10 (or 56 if absent).
static int __stdcall DispatchLinkedMoveTask(int a1, int /*a2*/, int /*a3*/)
{
    void* self = reinterpret_cast<void*>(static_cast<uintptr_t>(a1));

    int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    uint8_t* linkedBase = linkedValue
        ? reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(linkedValue - 4))
        : nullptr;

    void* subObject = CSF_AT(void*, linkedBase, 0x2C);
    void* innerObject = CSF_AT(void*, subObject, 0x34);
    uint8_t noTargetFlag = CSF_AT(uint8_t, innerObject, 0x39);

    if (noTargetFlag != 0)
        return Dispatch_NoLinkedTarget(self, nullptr, 5);

    int linkedField10 = GetLinkedObjectField10(self, nullptr);
    int resetArg = linkedField10
        ? (linkedField10 - 4) + 0x38
        : 0x38;

    return AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, resetArg);
}

HOOK(0x004AA760, DispatchLinkedMoveTask, "auto");
