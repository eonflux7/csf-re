#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_00444180_ForwardWithPackedArgs, 0x00444270, (S_00444180* self, void* , int a2, int* a3));
CSF_FUNC(int*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, TFncAtacarDesdeParapeto_SelectAttackState, 0x004B6F70, (TFncAtacarDesdeParapeto* self, void* , TAiState* state));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall TFncAtacarDesdeParapeto_InitializeAttackState(
    TFncAtacarDesdeParapeto* self, void* /*edx*/, TAiState* state)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    auto* actor = reinterpret_cast<uint8_t*>(
        linkedValue ? static_cast<uint32_t>(linkedValue) - 4 : 0);
    CSF_AT(int, actor, 0x6DC) = 0;

    linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    actor = reinterpret_cast<uint8_t*>(
        linkedValue ? static_cast<uint32_t>(linkedValue) - 4 : 0);
    int* position = GetObjectPositionVector(
        CSF_AT(void*, actor, 0x2C), nullptr);

    alignas(8) uint8_t packedArgs[0x14];
    CSF_AT(int, packedArgs, 0x0) = 0;
    CSF_AT(int, packedArgs, 0x4) = 0;
    CSF_AT(int, packedArgs, 0x8) = 0;
    CSF_AT(int, packedArgs, 0x0) = position[0];
    CSF_AT(int, packedArgs, 0x4) = position[1];
    CSF_AT(int, packedArgs, 0x8) = position[2];
    CSF_AT(float, packedArgs, 0xC) = 50.0f;
    CSF_AT(float, packedArgs, 0x10) = 200.0f;

    linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    actor = reinterpret_cast<uint8_t*>(
        linkedValue ? static_cast<uint32_t>(linkedValue) - 4 : 0);
    auto* object = CSF_AT(uint8_t*, actor, 0x2C);
    auto* sceneService = CSF_AT(S_00444180*, g_pCurrentScene, 0xB8);
    S_00444180_ForwardWithPackedArgs(
        sceneService, nullptr, reinterpret_cast<int>(object + 0x1C),
        reinterpret_cast<int*>(packedArgs));

    linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    actor = reinterpret_cast<uint8_t*>(
        linkedValue ? static_cast<uint32_t>(linkedValue) - 4 : 0);
    CSF_AT(uint8_t, actor, 0x6B0) = 0;

    linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    actor = reinterpret_cast<uint8_t*>(
        linkedValue ? static_cast<uint32_t>(linkedValue) - 4 : 0);
    if (!CSF_AT(int, actor, 0x6C8) ||
        CSF_AT(int, actor, 0x6C0) == -1 ||
        CSF_AT(int, actor, 0x6C4) == -1)
    {
        return CoverAttack_SetState_4DC150(
            state, nullptr,
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 4));
    }

    linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    actor = reinterpret_cast<uint8_t*>(
        linkedValue ? static_cast<uint32_t>(linkedValue) - 4 : 0);
    if (CSF_AT(uint8_t, actor, 0x6CC))
    {
        return CoverAttack_SetState_4DC150(
            state, nullptr,
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0xC));
    }

    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    if (static_cast<uint32_t>(GetStateValue(globalState, nullptr, 100)) >= 0x32u)
        return TFncAtacarDesdeParapeto_SelectAttackState(self, nullptr, state);

    return CoverAttack_SetState_4DC150(
        state, nullptr,
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x20));
}

HOOK(0x004BBF00, TFncAtacarDesdeParapeto_InitializeAttackState, "auto");
