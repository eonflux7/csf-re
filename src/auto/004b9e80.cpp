#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, AiDistracted_ProcessSceneObject, 0x00449510, (void* self, void* edx, int a2));
CSF_FUNC(C_008319c0*, __fastcall, S_004aef70_ResolvePendingTarget, 0x004AF100, (S_004aef70* self, void*));
CSF_FUNC(unsigned int, __fastcall, CoverAttack_Sub_4B9BA0, 0x004B9BA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_004D10F0_ResetFields, 0x004D10F0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));
CSF_FUNC(char*, __cdecl, AiDistracted_GetFutureErrorText, 0x007613D0, (int a1));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall TDistraido_EnterState(
    TDistraido* self, void* /*edx*/, TAiState* state)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    uintptr_t ownerAddress = linkedValue
        ? static_cast<uintptr_t>(linkedValue) - 4
        : 0;
    void* actor = CSF_AT(void*, reinterpret_cast<void*>(ownerAddress), 0x2C);
    void* actorData = CSF_AT(void*, actor, 0x34);
    AiDistracted_GetFutureErrorText(CSF_AT(int, actorData, 0x2F8));

    linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    ownerAddress = linkedValue ? static_cast<uintptr_t>(linkedValue) - 4 : 0;
    S_004aef70_ResolvePendingTarget(
        reinterpret_cast<S_004aef70*>(ownerAddress), nullptr);

    linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    ownerAddress = linkedValue ? static_cast<uintptr_t>(linkedValue) - 4 : 0;
    C_004D10F0_ResetFields(
        reinterpret_cast<void*>(ownerAddress + 1680), nullptr);

    linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    ownerAddress = linkedValue ? static_cast<uintptr_t>(linkedValue) - 4 : 0;
    CoverAttack_Sub_4B9BA0(reinterpret_cast<void*>(ownerAddress), nullptr);

    linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    ownerAddress = linkedValue ? static_cast<uintptr_t>(linkedValue) - 4 : 0;
    CSF_AT(uint8_t, reinterpret_cast<void*>(ownerAddress), 0x6CC) = 0;

    linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    ownerAddress = linkedValue ? static_cast<uintptr_t>(linkedValue) - 4 : 0;
    actor = CSF_AT(void*, reinterpret_cast<void*>(ownerAddress), 0x2C);
    void* sceneObject = CSF_AT(void*, g_pCurrentScene, 0xC4);
    AiDistracted_ProcessSceneObject(
        reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(sceneObject) + 4),
        nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(actor) + 28));

    return CoverAttack_SetState_4DC150(
        state, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(self) + 4));
}

HOOK(0x004B9E80, TDistraido_EnterState, "auto");
