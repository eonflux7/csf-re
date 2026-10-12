#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(double, __fastcall, ComputeLinkedSubObjectRatio, 0x004A9CD0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, DispatchSceneObjectWithPayload, 0x0048E4D0, (void* self, void* edx, float payload));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_PayloadScale, 0x0083543C);

static int __stdcall StaticAi_DispatchScaledLinkedTaskAndReset(int a1, int a2, int a3)
{
    void* self = reinterpret_cast<void*>(static_cast<uintptr_t>(a1));

    const int linkedField10 = GetLinkedObjectField10(self, nullptr);
    const float ratio = static_cast<float>(
        ComputeLinkedSubObjectRatio(reinterpret_cast<void*>(static_cast<uintptr_t>(linkedField10 + 0x30)), nullptr));

    const int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    const int linkedBaseAddr = linkedValue ? linkedValue - 4 : 0;
    uint8_t* linkedBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(linkedBaseAddr));
    void* dispatchSelf = CSF_AT(void*, linkedBase, 0x2C);
    DispatchSceneObjectWithPayload(dispatchSelf, nullptr, ratio * g_PayloadScale);

    const int linkedField10Again = GetLinkedObjectField10(self, nullptr);
    return AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, linkedField10Again + 0x0C);
}

HOOK(0x004B0610, StaticAi_DispatchScaledLinkedTaskAndReset, "auto");
