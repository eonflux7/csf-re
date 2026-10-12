#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateCharacterTasks, 0x004DBD50, (int a1, int a2));
CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(char, __fastcall, AiCoverAttackTasks_IsMovementComplete, 0x004B9950, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

static char __stdcall AiDistracted_UpdateCoverTaskAndResetLinked(int a1, int a2)
{
    void* self = reinterpret_cast<void*>(a1);
    AiCoverAttackTasks_UpdateCharacterTasks(a1, a2);

    char result = static_cast<char>(IsField10Null(self, nullptr));
    if (!result) {
        int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
        void* linkedObject = linkedValue
            ? reinterpret_cast<void*>(linkedValue - 4)
            : nullptr;

        result = AiCoverAttackTasks_IsMovementComplete(linkedObject, nullptr);
        if (result) {
            int linkedField10 = GetLinkedObjectField10(self, nullptr);
            return static_cast<char>(
                AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, linkedField10 + 36));
        }
    }
    return result;
}

HOOK(0x004BC070, AiDistracted_UpdateCoverTaskAndResetLinked, "auto");
