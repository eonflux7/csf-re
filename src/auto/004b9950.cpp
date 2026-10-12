#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, IsDistinctNonSentinelValue, 0x0048E0D0, (void* self, void* , std::int32_t candidate));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(char, __fastcall, C_0083e9d0_FindPairLink, 0x004E74E0, (C_0083e9d0* self, void* , int kind, int target, uint32_t* out));
CSF_FUNC(int, __cdecl, GameConfig_IsSceneObjectRemovable, 0x007558C0, (int objectType));

CSF_GLOBAL(float, g_CoverAttackNearDistanceSquared, 0x0083A9DC);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall AiDistracted_TrySelectNearbyAttackTarget(
    char* self, void* /*edx*/)
{
    // The pair result and its embedded handle share one contiguous output buffer.
    alignas(8) std::uint8_t pairBytes[0x3C];
    std::uint8_t* handleBytes = pairBytes + 0x14;

    CSF_AT(void**, pairBytes, 0) = CSF_VTBL(0x0083124C);
    CSF_AT(void**, handleBytes, 0) = CSF_VTBL(0x0082D448);
    CSF_AT(std::int32_t, handleBytes, 0x4) = -1;
    CSF_AT(std::int32_t, handleBytes, 0x8) = -1;
    std::memset(handleBytes + 0xC, 0, 12);
    std::memset(pairBytes + 0x4, 255, 16);
    CSF_AT(float, pairBytes, 0x2C) = 3.4028235e38f;
    CSF_AT(std::int32_t, pairBytes, 0x30) = 0;

    int linkedValue = GetLinkedObjectValue_4DB5C0(self + 8, nullptr);
    void* linkedObject = linkedValue
        ? reinterpret_cast<std::uint8_t*>(linkedValue) - 4
        : nullptr;
    void* owner = CSF_AT(void*, linkedObject, 0x2C);

    if (!C_0083e9d0_FindPairLink(
            CSF_AT(C_0083e9d0*, owner, 0x3C), nullptr, 2, 6,
            reinterpret_cast<uint32_t*>(pairBytes)))
        return 0;

    void* target = reinterpret_cast<void*>(CHandleTable_Resolve(
        g_pCurrentScene, nullptr, reinterpret_cast<int>(handleBytes)));
    if (!target)
        return 0;

    int objectType = CSF_AT(int, target, 0x50);
    if (objectType == 17)
        return 0;

    if (!(CSF_AT(float, pairBytes, 0x2C) < g_CoverAttackNearDistanceSquared))
        return 0;

    if (!static_cast<std::uint8_t>(
            GameConfig_IsSceneObjectRemovable(objectType)))
        return 0;

    if (CSF_AT(std::int32_t, target, 0x190) < 0)
        return 0;

    void* relatedObject = reinterpret_cast<void*>(CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        reinterpret_cast<int>(static_cast<std::uint8_t*>(target) + 0x188)));
    if (!relatedObject)
        return 0;

    linkedValue = GetLinkedObjectValue_4DB5C0(self + 8, nullptr);
    linkedObject = linkedValue
        ? reinterpret_cast<std::uint8_t*>(linkedValue) - 4
        : nullptr;
    owner = CSF_AT(void*, linkedObject, 0x2C);

    if (!static_cast<std::uint8_t>(IsDistinctNonSentinelValue(
            owner, nullptr, CSF_AT(std::int32_t, relatedObject, 0x78))))
        return 0;

    CSF_AT(std::uint32_t, self, 0x6D4) =
        CSF_AT(std::uint32_t, target, 0x20);
    CSF_AT(std::uint32_t, self, 0x6D8) =
        CSF_AT(std::uint32_t, target, 0x24);
    return 1;
}

HOOK(0x004B9950, AiDistracted_TrySelectNearbyAttackTarget, "auto");
