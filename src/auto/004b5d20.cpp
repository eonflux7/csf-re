#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __fastcall, CGuiControl_GetField012C, 0x0044F630, (CGuiControl* self, void*));
CSF_FUNC(uint32_t*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiCoverAttackTasks_GetPositionData, 0x004A9CA0, (S_004abb90* self, void*));
CSF_FUNC(int, __fastcall, StaticGuard_CanEngageTarget, 0x004AEE30, (S_004aa310* self, void* , S_004abb90* a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(void*, __fastcall, InitDefaultFieldBlock, 0x004DEF80, (void* self, void*));
CSF_FUNC(char, __fastcall, S_004dfbc0_TrySetupSubObjectPair, 0x004E07B0, (S_004dfbc0* self, void* , unsigned int a2, int a3));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(int, g_AiSearchCostThisFrame, 0x008B044C);

static char __fastcall S_004abb90_CommitSearchCandidate(
    S_004abb90* self, void* /*edx*/, int candidate)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<void*>(candidate), nullptr);
    auto* linkedObject = linkedValue
        ? reinterpret_cast<S_004aa310*>(static_cast<uint32_t>(linkedValue) - 4)
        : nullptr;
    auto* searchPosition = reinterpret_cast<S_004abb90*>(
        reinterpret_cast<uint8_t*>(self) + 0x38);

    if (static_cast<uint8_t>(
            StaticGuard_CanEngageTarget(linkedObject, nullptr, searchPosition)) == 0)
        return 0;

    alignas(8) uint8_t resultBlock[0x4C];
    alignas(8) uint8_t requestBlock[0x30];

    InitDefaultFieldBlock(resultBlock, nullptr);

    CSF_AT(uint32_t, requestBlock, 0x00) = 0;
    CSF_AT(uint32_t, requestBlock, 0x04) = 0;
    CSF_AT(uint32_t, requestBlock, 0x08) = 0;
    CSF_AT(uint32_t, requestBlock, 0x0C) = 0;
    CSF_AT(uint32_t, requestBlock, 0x10) = 0;
    CSF_AT(uint32_t, requestBlock, 0x14) = 0;
    CSF_AT(uint32_t, requestBlock, 0x18) = 0;
    CSF_AT(uint32_t, requestBlock, 0x1C) = 0;
    CSF_AT(uint32_t, requestBlock, 0x20) = 0;
    CSF_AT(uint32_t, requestBlock, 0x24) = 2139095039;
    CSF_AT(uint32_t, requestBlock, 0x28) = 0;
    CSF_AT(uint32_t, requestBlock, 0x2C) = 0;

    linkedValue = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<void*>(candidate), nullptr);
    void* linkedBase = linkedValue
        ? reinterpret_cast<void*>(static_cast<uint32_t>(linkedValue) - 4)
        : nullptr;

    uint32_t* position = GetObjectPositionVector(
        CSF_AT(void*, linkedBase, 0x2C), nullptr);
    CSF_AT(uint32_t, requestBlock, 0x00) = position[0];
    CSF_AT(uint32_t, requestBlock, 0x04) = position[1];
    CSF_AT(uint32_t, requestBlock, 0x08) = position[2];

    uint32_t* searchPositionData =
        AiCoverAttackTasks_GetPositionData(searchPosition, nullptr);
    CSF_AT(uint32_t, requestBlock, 0x0C) = searchPositionData[0];
    CSF_AT(uint32_t, requestBlock, 0x10) = searchPositionData[1];
    CSF_AT(uint32_t, requestBlock, 0x14) = searchPositionData[2];

    void* globalState = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    CSF_AT(uint32_t, requestBlock, 0x24) =
        CSF_AT(uint32_t, globalState, 0xAB4);

    uint32_t sceneField = CGuiControl_GetField012C(
        reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr);
    int setupObject = GetCurrentItemCount(
        reinterpret_cast<void*>(sceneField), nullptr);
    char success = S_004dfbc0_TrySetupSubObjectPair(
        reinterpret_cast<S_004dfbc0*>(setupObject), nullptr,
        reinterpret_cast<unsigned int>(requestBlock),
        reinterpret_cast<int>(resultBlock));

    g_AiSearchCostThisFrame = static_cast<int>(
        static_cast<uint32_t>(g_AiSearchCostThisFrame) + 10);

    if (CSF_AT(void*, resultBlock, 0x3C))
    {
        CoreMemory_Free(CSF_AT(void*, resultBlock, 0x3C));
        CSF_AT(void*, resultBlock, 0x3C) = nullptr;
    }

    void* ownedBlock = CSF_AT(void*, resultBlock, 0x08);
    CSF_AT(int, resultBlock, 0x10) = 0;
    CSF_AT(int, resultBlock, 0x18) = -1082130432;
    if (ownedBlock)
        CoreMemory_Free(ownedBlock);

    return success;
}

HOOK(0x004B5D20, S_004abb90_CommitSearchCandidate, "auto");
