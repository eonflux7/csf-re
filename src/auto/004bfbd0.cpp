#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_RegisterPosition, 0x00443B50, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AppendCommandScale, 0x004BC2A0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// The packet also lists the direct capacity-growth callee as a vtable reference.
static const auto g_ArrayCapacityReference = CSF_VTBL(0x0041E5F0);

static int __stdcall AiCore_ResetLinkedActorCommands(int actorLink)
{
    void* link = reinterpret_cast<void*>(actorLink);

    uint32_t linkedValue = static_cast<uint32_t>(
        GetLinkedObjectValue_4DB5C0(link, nullptr));
    void* actor = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);
    CSF_AT(uint32_t, actor, 0x6DC) = 0;

    linkedValue = static_cast<uint32_t>(
        GetLinkedObjectValue_4DB5C0(link, nullptr));
    actor = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);
    uint32_t positionAddress = CSF_AT(uint32_t, actor, 0x2C) + 28;
    AiCoverAttackTasks_RegisterPosition(
        CSF_AT(void*, g_pCurrentScene, 0xB8), nullptr,
        static_cast<int>(positionAddress));

    linkedValue = static_cast<uint32_t>(
        GetLinkedObjectValue_4DB5C0(link, nullptr));
    actor = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);
    void* sceneObject = CSF_AT(void*, actor, 0x2C);
    ((void(__fastcall*)(void*, void*, int, int, int, int))
        CSF_AT(void**, sceneObject, 0)[0x8C / 4])(
            sceneObject, nullptr, 0, 0, -1, 0);

    linkedValue = static_cast<uint32_t>(
        GetLinkedObjectValue_4DB5C0(link, nullptr));
    actor = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);
    UnnamedApply_4DC180(
        link, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(actor) + 1088));

    linkedValue = static_cast<uint32_t>(
        GetLinkedObjectValue_4DB5C0(link, nullptr));
    actor = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);
    auto* commands = reinterpret_cast<S_0041e5f0*>(
        reinterpret_cast<uintptr_t>(actor) + 1292);

    if (commands->field_0008 >= commands->field_0004)
    {
        S_0041e5f0_GrowArrayCapacity(
            commands, nullptr, static_cast<int>(commands->field_000c));
    }

    uint32_t commandIndex = commands->field_0008;
    auto* command = reinterpret_cast<uint32_t*>(
        commands->field_0000 + 4 * commandIndex);
    commands->field_0008 = commandIndex + 1;
    *command = 1;

    return AppendCommandScale(commands, nullptr, 1065353216);
}

HOOK(0x004BFBD0, AiCore_ResetLinkedActorCommands, "auto");
