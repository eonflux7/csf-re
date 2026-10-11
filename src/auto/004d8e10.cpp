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

// Original callee: __thiscall, 1 stack arg, ret 4 (bound with the packet's exact signature)
CSF_FUNC(int, __fastcall, AiAttackTasks_CopyTargetHandle, 0x0048C9F0, (void* self, void* edx, int a2));

// Original callee: __thiscall, no stack args, ret 0
CSF_FUNC(void, __fastcall, ResetAttackTaskState, 0x0047B220, (void* self, void* edx));

// Clears the attack target of the autonomous AI object when a target handle is set.
// Copies a "no target" handle into the owner's attack-task controller, then resets the
// attack task state stored at this+0x18.
static int __fastcall AiAutonomous_ClearAttackTarget(int* self, void* /*edx*/)
{
    int result = CSF_AT(int, self, 0x18);
    if (result != 0)
    {
        // Stack handle { vtable, -1, -1 } passed by address to the copy routine.
        alignas(8) uint32_t targetHandle[3];
        targetHandle[0] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448)));
        targetHandle[1] = 0xFFFFFFFFu;
        targetHandle[2] = 0xFFFFFFFFu;

        // owner = this->+0x08; controller = owner->+0x14; attackTasks = controller->+0x204
        void* owner = CSF_AT(void*, self, 0x08);
        void* controller = CSF_AT(void*, owner, 0x14);
        void* attackTasks = CSF_AT(void*, controller, 0x204);

        AiAttackTasks_CopyTargetHandle(attackTasks, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(targetHandle)));

        // The original re-reads this+0x18 after the copy call.
        ResetAttackTaskState(CSF_AT(void*, self, 0x18), nullptr);
        return 0;
    }
    return result;
}

HOOK(0x004D8E10, AiAutonomous_ClearAttackTarget, "auto");
