#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S_00448a80_ProcessSceneObject, 0x00449510, (S_00448a80* self, void* , S_00448a80* sceneObject));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));
CSF_FUNC(void, __stdcall, PrepareStaticAiState, 0x004DC3C0, (void* a1));
CSF_FUNC(unsigned int, __fastcall, DispatchWithArgBlock, 0x004DD060, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static int __fastcall AiCombat_DispatchLinkedObjectAndSetState(
    void* self, void* /*edx*/, void* aiState)
{
    alignas(8) uint8_t argumentBytes[0x1C];

    PrepareStaticAiState(aiState);

    auto* context = reinterpret_cast<uint8_t*>(GetValue8B0554());
    auto* dispatcher = CSF_AT(uint8_t*, context, 0x1FD8);

    int linkedValue = GetLinkedObjectValue_4DB5C0(aiState, nullptr);
    auto* linkedObject = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;
    auto* sceneObject = CSF_AT(uint8_t*, linkedObject, 0x2C) + 28;

    CSF_AT(uint32_t, argumentBytes, 0x00) = 0;
    CSF_AT(uint32_t, argumentBytes, 0x04) =
        reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    CSF_AT(uint32_t, argumentBytes, 0x08) =
        CSF_AT(uint32_t, sceneObject, 0x04);
    CSF_AT(uint32_t, argumentBytes, 0x0C) =
        CSF_AT(uint32_t, sceneObject, 0x08);
    CSF_AT(float, argumentBytes, 0x10) = g_ZeroVectorX;
    CSF_AT(float, argumentBytes, 0x14) = g_ZeroVectorY;
    CSF_AT(float, argumentBytes, 0x18) = g_ZeroVectorZ;

    DispatchWithArgBlock(
        dispatcher + 68, nullptr,
        reinterpret_cast<uint32_t*>(argumentBytes));

    linkedValue = GetLinkedObjectValue_4DB5C0(aiState, nullptr);
    linkedObject = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;
    sceneObject = CSF_AT(uint8_t*, linkedObject, 0x2C) + 28;

    auto* sceneProcessor =
        CSF_AT(uint8_t*, g_pCurrentScene, 0xC4) + 4;
    S_00448a80_ProcessSceneObject(
        reinterpret_cast<S_00448a80*>(sceneProcessor), nullptr,
        reinterpret_cast<S_00448a80*>(sceneObject));

    return CoverAttack_SetState_4DC150(
        aiState, nullptr,
        reinterpret_cast<int>(static_cast<uint8_t*>(self) + 44));
}

HOOK(0x004B97A0, AiCombat_DispatchLinkedObjectAndSetState, "auto");
