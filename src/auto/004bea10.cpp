#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __fastcall, CoverAttack_SetStance_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, UnnamedSetRangeParams_4BC3A0, 0x004BC3A0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, AiCore_BuildDefaultCoverAttackTasks, 0x004BE750, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_SetCoverTransition, 0x004D1100, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

// Retain the packet's listed reference; this address is the growth function.
[[maybe_unused]] static const auto k_ArrayGrowthReference = CSF_VTBL(0x0041E5F0);

static int __stdcall AiCore_InitializeCoverAttackState(int aiAddress, int /*unused*/)
{
    void* ai = reinterpret_cast<void*>(static_cast<std::uintptr_t>(aiAddress));

    int linked = GetLinkedObjectValue_4DB5C0(ai, nullptr);
    std::uintptr_t owner = linked
        ? static_cast<std::uintptr_t>(linked) - 4u : 0u;

    void* globalState = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(GetGlobalValue_8BC1E0()));
    AiCoverAttackTasks_SetCoverTransition(
        reinterpret_cast<void*>(owner + 0x690), nullptr,
        0, CSF_AT(int, globalState, 0xA8C));

    linked = GetLinkedObjectValue_4DB5C0(ai, nullptr);
    owner = linked ? static_cast<std::uintptr_t>(linked) - 4u : 0u;
    CoverAttack_SetStance_492720(
        CSF_AT(void*, reinterpret_cast<void*>(owner), 0x2C), nullptr, 0);

    linked = GetLinkedObjectValue_4DB5C0(ai, nullptr);
    owner = linked ? static_cast<std::uintptr_t>(linked) - 4u : 0u;
    void* actor = CSF_AT(void*, reinterpret_cast<void*>(owner), 0x2C);
    reinterpret_cast<void (__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, actor, 0)[0x8C / 4])(
            actor, nullptr, 0, 0, -1, 0);

    linked = GetLinkedObjectValue_4DB5C0(ai, nullptr);
    owner = linked ? static_cast<std::uintptr_t>(linked) - 4u : 0u;
    CSF_AT(std::uint32_t, reinterpret_cast<void*>(owner), 0x680) |= 1u;

    linked = GetLinkedObjectValue_4DB5C0(ai, nullptr);
    owner = linked ? static_cast<std::uintptr_t>(linked) - 4u : 0u;
    UnnamedApply_4DC180(ai, nullptr, static_cast<int>(owner + 0x440));

    linked = GetLinkedObjectValue_4DB5C0(ai, nullptr);
    owner = linked ? static_cast<std::uintptr_t>(linked) - 4u : 0u;
    const std::uintptr_t taskState = owner + 0x440;

    linked = GetLinkedObjectValue_4DB5C0(ai, nullptr);
    owner = linked ? static_cast<std::uintptr_t>(linked) - 4u : 0u;
    if (CSF_AT(int, reinterpret_cast<void*>(owner), 0x608) == -1 &&
        CSF_AT(int, reinterpret_cast<void*>(owner), 0x60C) == -1)
    {
        linked = GetLinkedObjectValue_4DB5C0(ai, nullptr);
        owner = linked ? static_cast<std::uintptr_t>(linked) - 4u : 0u;
        return AiCore_BuildDefaultCoverAttackTasks(
            reinterpret_cast<void*>(owner), nullptr, static_cast<int>(taskState));
    }

    auto* tasks = reinterpret_cast<S_0041e5f0*>(taskState + 0xCC);
    if (tasks->field_0008 >= tasks->field_0004)
    {
        S_0041e5f0_GrowArrayCapacity(
            tasks, nullptr, static_cast<int>(tasks->field_000c));
    }

    const std::uint32_t count = tasks->field_0008;
    auto* entry = reinterpret_cast<std::uint32_t*>(
        tasks->field_0000 + 4u * count);
    tasks->field_0008 = count + 1u;
    *entry = 31;
    return UnnamedSetRangeParams_4BC3A0(tasks, nullptr, 0.0f, 1.0f);
}

HOOK(0x004BEA10, AiCore_InitializeCoverAttackState, "auto");
