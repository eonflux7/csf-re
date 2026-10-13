#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

// Looks up the object linked to `self` (field at +10), then resets the task
// state stored at offset +8 of that linked object, using `self` as `this`.
static int __stdcall AiCoverAttackTasks_ResetLinkedObjectTaskState(void* self)
{
    int linkedObjectField10 = GetLinkedObjectField10(self, nullptr);
    return AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, linkedObjectField10 + 8);
}

HOOK(0x004AD7C0, AiCoverAttackTasks_ResetLinkedObjectTaskState, "auto");
