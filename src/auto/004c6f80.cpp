#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, GetPositionCategory, 0x0044A950, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AppendCommandScale, 0x004BC2A0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AppendTimingParameters, 0x004BC3A0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, AppendOffsetParameters, 0x004BC610, (void* self, void* edx, float a2, int a3));
CSF_FUNC(int, __fastcall, InitializePositionCommands, 0x004C5150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, SetLinkedTarget, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// The packet also identifies the array-growth address as a vtable reference.
[[maybe_unused]] static const auto kArrayGrowthReference = CSF_VTBL(0x0041E5F0);

static std::uintptr_t AiCoverAttackTasks_GetLinkedBase(void* reference)
{
    const int linked = GetLinkedObjectValue_4DB5C0(reference, nullptr);
    return linked ? static_cast<std::uint32_t>(linked) - 4 : 0;
}

static void AiCoverAttackTasks_AppendCommand(S_0041e5f0* commands, int command)
{
    if (commands->field_0008 >= commands->field_0004)
        GrowButtonArray(commands, nullptr, commands->field_000c);

    const std::uint32_t index = commands->field_0008;
    auto* destination =
        reinterpret_cast<int*>(commands->field_0000 + 4 * index);
    commands->field_0008 = index + 1;
    *destination = command;
}

static int __fastcall S_004c6f80_BuildCoverAttackCommands(
    S_004c6f80* self, void* /*edx*/, int linkedReference, char alternateMode)
{
    self->field_0004 = alternateMode;
    void* reference = reinterpret_cast<void*>(linkedReference);

    std::uintptr_t base = AiCoverAttackTasks_GetLinkedBase(reference);
    SetLinkedTarget(reference, nullptr, static_cast<int>(base + 1088));

    const std::uintptr_t commandOwner =
        AiCoverAttackTasks_GetLinkedBase(reference);
    base = AiCoverAttackTasks_GetLinkedBase(reference);

    const std::uintptr_t position =
        base + (CSF_AT(int, reinterpret_cast<void*>(base), 0x630) < 0
                    ? 1552
                    : 1576);
    auto* commands = reinterpret_cast<S_0041e5f0*>(commandOwner + 1292);
    InitializePositionCommands(commands, nullptr, static_cast<int>(position));

    if (self->field_0004)
    {
        AiCoverAttackTasks_AppendCommand(commands, 6);
    }
    else
    {
        AiCoverAttackTasks_AppendCommand(commands, 5);
        AiCoverAttackTasks_AppendCommand(commands, 10);
    }

    base = AiCoverAttackTasks_GetLinkedBase(reference);
    const std::uintptr_t actor =
        AiCoverAttackTasks_GetLinkedBase(reinterpret_cast<void*>(base + 8));
    const std::uint32_t positionObject =
        CSF_AT(std::uint32_t, reinterpret_cast<void*>(actor), 0x2C);
    void* positionManager = CSF_AT(void*, g_pCurrentScene, 0xC4);
    const int category = GetPositionCategory(
        positionManager, nullptr, static_cast<int>(positionObject + 28));

    float offset;
    if (category == 3)
    {
        base = AiCoverAttackTasks_GetLinkedBase(reference);
        CSF_AT(int, reinterpret_cast<void*>(base), 0x6DC) = 3;
        offset = 200.0f;
    }
    else if (category == 4)
    {
        base = AiCoverAttackTasks_GetLinkedBase(reference);
        CSF_AT(int, reinterpret_cast<void*>(base), 0x6DC) = 4;
        offset = -200.0f;
    }
    else
    {
        base = AiCoverAttackTasks_GetLinkedBase(reference);
        CSF_AT(int, reinterpret_cast<void*>(base), 0x6DC) = 0;
        const int state = GetGlobalState_50D190();
        offset = GetStateValue(reinterpret_cast<void*>(state), nullptr, 2)
                     ? -200.0f
                     : 200.0f;
    }

    AiCoverAttackTasks_AppendCommand(commands, 13);
    AppendOffsetParameters(commands, nullptr, offset, 1145569280);
    AppendTimingParameters(commands, nullptr, 2.0f, 5.0f);
    AiCoverAttackTasks_AppendCommand(commands, 14);
    return AppendCommandScale(commands, nullptr, 1065353216);
}

HOOK(0x004C6F80, S_004c6f80_BuildCoverAttackCommands, "auto");
