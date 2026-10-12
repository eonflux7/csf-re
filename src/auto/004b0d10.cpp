#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(float*, __fastcall, AiFncStatic_InitializePositionPair, 0x0040ECB0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30,
         (void* self, void* edx, int handle));
CSF_FUNC(char, __fastcall, AiCharTask_TestPositionPair, 0x004AF540,
         (void* self, void* edx, float* a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0,
         (void* self, void*));

CSF_GLOBAL(float, g_fReturnedConstant_0082E030, 0x0082E030);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall AiFncStatic_TrySetVisibleTargetPosition(
    void* self, void* /*edx*/, void* linkedHandle)
{
    void* target = reinterpret_cast<void*>(CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        reinterpret_cast<int>(static_cast<uint8_t*>(self) + 0x14)));

    if (!target ||
        (CSF_AT(uint32_t, target, 0x170) & 0x2000) == 0 ||
        CSF_AT(int, target, 0x1C4) != 1)
        return 0;

    alignas(8) uint8_t pairBytes[0x18];
    alignas(8) uint8_t resultBytes[0xC];
    alignas(8) uint8_t sourceBytes[0x18];
    alignas(8) uint8_t targetBytes[0xC];

    float* positionPair = reinterpret_cast<float*>(pairBytes);
    uint32_t* result = reinterpret_cast<uint32_t*>(resultBytes);
    float* sourcePosition = reinterpret_cast<float*>(sourceBytes);
    float* targetPosition = reinterpret_cast<float*>(targetBytes);

    int linkedValue = GetLinkedObjectValue_4DB5C0(linkedHandle, nullptr);
    void* linkedObject = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;
    void* sourceObject = CSF_AT(void*, linkedObject, 0x2C);

    reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
        CSF_AT(void**, sourceObject, 0)[0xA4 / 4])(
            sourceObject, nullptr, sourcePosition);
    reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
        CSF_AT(void**, target, 0)[0xA4 / 4])(
            target, nullptr, targetPosition);

    AiFncStatic_InitializePositionPair(positionPair, nullptr);

    sourcePosition[3] = targetPosition[0];
    sourcePosition[4] = targetPosition[1] + g_fReturnedConstant_0082E030;
    sourcePosition[5] = targetPosition[2];

    positionPair[1] = sourcePosition[1];
    positionPair[0] = sourcePosition[0];
    positionPair[2] = sourcePosition[2];
    positionPair[4] = sourcePosition[4];
    positionPair[3] = sourcePosition[3];
    positionPair[5] = sourcePosition[5];
    std::memset(resultBytes, 0, sizeof(resultBytes));

    linkedValue = GetLinkedObjectValue_4DB5C0(linkedHandle, nullptr);
    linkedObject = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;

    if (AiCharTask_TestPositionPair(
            linkedObject, nullptr, positionPair, result))
        return 0;

    CSF_AT(float, self, 0x20) = targetPosition[0];
    CSF_AT(float, self, 0x24) = targetPosition[1];
    CSF_AT(float, self, 0x28) = targetPosition[2];
    return 1;
}

HOOK(0x004B0D10, AiFncStatic_TrySetVisibleTargetPosition, "auto");
