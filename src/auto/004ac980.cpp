#include <cstdint>
#include "globals.h"

#include "hook.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

static char __stdcall AiStateFactory_ResetLinkedCoverTasks(int a1)
{
    void* self = reinterpret_cast<void*>(static_cast<uintptr_t>(a1));

    int linkedField10 = GetLinkedObjectField10(self, nullptr);
    AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, linkedField10 + 0x30);
    return 1;
}

HOOK(0x004AC980, AiStateFactory_ResetLinkedCoverTasks, "auto");
