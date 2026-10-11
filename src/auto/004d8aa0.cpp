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

// Base-class helper: clears the task's timer slots (fields +0x00 .. +0x0C of the owner chain).
CSF_FUNC(int, __fastcall, TCharTask_PrepareBase, 0x004DB3F0, (void* self, void* edx));

// Starts a char action (task type 7) on the owner's inner AI object.
CSF_FUNC(int, __fastcall, TaskCharAction_48CFE0, 0x0048CFE0, (void* self, void* edx, int a2, char a3, char a4));

// Copies the target handle (vtable C_0082d448) into the AI attack-task object.
CSF_FUNC(int, __fastcall, AiAttackTasks_CopyTargetHandle, 0x0048C9F0, (void* self, void* edx, int a2));

static int __fastcall TCharTask_ResetAndAttachTargetHandle(TCharTask* self, void* /*edx*/)
{
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;
    TCharTask_PrepareBase(self, nullptr);

    // First call: the inner AI object is this->owner->+0x14->+0x204.
    TaskCharAction_48CFE0(
        *reinterpret_cast<TCharTask**>(CSF_AT(uint32_t, CSF_AT(uint32_t, self, 0x08), 0x14) + 0x204),
        nullptr,
        (int)(uintptr_t)self,
        7,
        1);

    // Target handle object on the stack: [vtbl, -1, -1].
    alignas(8) uint8_t handleBytes[12];
    int32_t* handleWords = reinterpret_cast<int32_t*>(handleBytes);
    handleWords[1] = -1;
    handleWords[2] = -1;
    *reinterpret_cast<void**>(handleBytes) = CSF_VTBL(0x0082D448);

    // Second call re-reads the owner chain (callees may have changed it).
    return AiAttackTasks_CopyTargetHandle(
        *reinterpret_cast<void**>(CSF_AT(uint32_t, CSF_AT(uint32_t, self, 0x08), 0x14) + 0x204),
        nullptr,
        (int)(uintptr_t)handleBytes);
}

HOOK(0x004D8AA0, TCharTask_ResetAndAttachTargetHandle, "auto");
