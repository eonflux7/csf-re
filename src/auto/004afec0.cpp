#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateCharacterTasks, 0x004DBD50, (int a1, int a2));
CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(bool, __fastcall, CoverTask_IsLinkedObjectReady, 0x004AF3D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

// TEstatico: updates the character's cover/attack tasks, then, if the linked
// object is ready, resets its linked task state.
static char __stdcall AiStatic_UpdateCoverTaskAndResetLinked(int a1, int a2) {
    void* self = reinterpret_cast<void*>(a1);

    AiCoverAttackTasks_UpdateCharacterTasks(a1, a2);

    char result = (char)IsField10Null(self, nullptr);
    if (!result) {
        int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
        void* linkedObj = linkedValue ? reinterpret_cast<void*>(linkedValue - 4) : nullptr;

        result = (char)CoverTask_IsLinkedObjectReady(linkedObj, nullptr);
        if (result) {
            int linkedField10 = GetLinkedObjectField10(self, nullptr);
            return (char)AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, linkedField10 + 20);
        }
    }
    return result;
}

HOOK(0x004AFEC0, AiStatic_UpdateCoverTaskAndResetLinked, "auto");
