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

// Callees (bound by address, signatures as given in the packet)
CSF_FUNC(char, __fastcall, SceneEntity_Activate, 0x004E51B0, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, TaskCharAction_48CFE0, 0x0048CFE0, (void* self, void* edx, int a2, char a3, char a4));
CSF_FUNC(char, __fastcall, AiAttackTasks_SetActionFlag, 0x0048C9D0, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, AiAttackTasks_CopyTargetHandle, 0x0048C9F0, (void* self, void* edx, int a2));

// Starts the attack task of an autonomous AI: optionally deactivates the scene entity
// when the owner's state is 2, starts char action 5 on the task, sets the action flag,
// and copies the target handle block (vtable + two -1 slots) into the attack task.
static int __fastcall AiAutonomous_BeginAttackTask(void* self, void* /*edx*/)
{
    void* owner = CSF_AT(void*, self, 0x08);

    if (CSF_AT(int, owner, 0x2F8) == 2)
        SceneEntity_Activate(CSF_AT(void*, CSF_AT(void*, owner, 0x14), 0x3C), nullptr, 0);

    void* charTask = CSF_AT(void*, CSF_AT(void*, owner, 0x14), 0x204);
    TaskCharAction_48CFE0(charTask, nullptr, reinterpret_cast<int>(self), 5, 1);

    charTask = CSF_AT(void*, CSF_AT(void*, self, 0x08) /* owner again */, 0);
    (void)charTask;
    AiAttackTasks_SetActionFlag(CSF_AT(void*, CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x14), 0x204), nullptr, 1);

    // Target handle block: [0] = vtable of C_0082d448, [1] and [2] = -1
    int handleBlock[3];
    handleBlock[1] = -1;
    handleBlock[2] = -1;
    handleBlock[0] = static_cast<int>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448)));

    // Reference to the target task's vtable (S_0048c9f0), used as the object of CopyTargetHandle
    [[maybe_unused]] auto* targetHandleVtbl = CSF_VTBL(0x0048C9F0);

    void* target = CSF_AT(void*, CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x14), 0x204);
    return AiAttackTasks_CopyTargetHandle(target, nullptr,
                                          static_cast<int>(reinterpret_cast<uintptr_t>(handleBlock)));
}

HOOK(0x004D8860, AiAutonomous_BeginAttackTask, "auto");
