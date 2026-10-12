#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

// Looks up the object linked to `self` (field at +10), then resets the task
// state stored at offset +8 of that linked object, using `self` as `this`.
// The original is __stdcall with three stack args (ret 12); a2 and a3 are unused.
static int __stdcall AiMoveTasks_ResetLinkedObjectTaskStateAtOffset8(void* self, int /*a2*/, int /*a3*/)
{
    int linkedObjectField10 = GetLinkedObjectField10(self, nullptr);
    return AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, linkedObjectField10 + 8);
}

HOOK(0x004AB060, AiMoveTasks_ResetLinkedObjectTaskStateAtOffset8, "auto");
