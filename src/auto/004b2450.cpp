#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, IsDistinctNonSentinelValue, 0x0048E0D0, (void* self, void* , std::int32_t candidate));
CSF_FUNC(int, __fastcall, AiCore_IsTargetRecordValid, 0x004B2290, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall S_004ad990_ResolveCombatTarget(
    S_004ad990* self, void* /*edx*/, int handle)
{
    if (CSF_AT(std::int32_t, reinterpret_cast<void*>(handle), 0x8) < 0)
        return 0;

    auto* target = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, handle));
    if (!target)
        return 0;

    void* owner = reinterpret_cast<void*>(self->field_0000);
    if (static_cast<void*>(target) == owner)
        return 0;

    if (static_cast<std::uint8_t>(IsDistinctNonSentinelValue(
            owner, nullptr, static_cast<std::int32_t>(target->field_0078))))
        return 0;

    void* attached = reinterpret_cast<void*>(target->field_0034);
    if (!attached)
        return 0;

    if (reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, attached, 0)[0x98 / 4])(attached, nullptr) != 1)
        return 0;

    attached = reinterpret_cast<void*>(target->field_0034);
    auto* record = reinterpret_cast<S_004ad990*>(
        static_cast<std::uint8_t*>(CSF_AT(void*, attached, 0x30C)) + 0x5DC);

    if (!static_cast<std::uint8_t>(AiCore_IsTargetRecordValid(record, nullptr)))
        return 0;

    std::uint8_t* selectedHandle = reinterpret_cast<std::uint8_t*>(record) + 0x4C;
    if (static_cast<std::int32_t>(record->field_0054) < 0)
        selectedHandle = reinterpret_cast<std::uint8_t*>(record) + 0x34;

    owner = reinterpret_cast<void*>(self->field_0000);
    if (CSF_AT(std::uint32_t, selectedHandle, 0x4) ==
        CSF_AT(std::uint32_t, owner, 0x20))
        return 0;

    return static_cast<int>(reinterpret_cast<std::intptr_t>(record));
}

HOOK(0x004B2450, S_004ad990_ResolveCombatTarget, "auto");
