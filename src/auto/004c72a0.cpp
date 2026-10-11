#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_AppendDestinationCommands, 0x004BC300, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_AppendCoverPositionCommands, 0x004C5150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_SetMovementContext, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_QuarterTurnConstant, 0x0082FAD4);

static void __stdcall AiCoverAttackTasks_ConfigureCoverMovementCommands(
    int object, char addExtraCommands, char addRandomDestination)
{
    void* self = reinterpret_cast<void*>(object);

    // The packet labels the direct growth-call address as a vtable reference.
    (void)CSF_VTBL(0x0041E5F0);

    int linked = GetLinkedObjectValue_4DB5C0(self, nullptr);
    int owner = linked ? linked - 4 : 0;
    AiCoverAttackTasks_SetMovementContext(self, nullptr, owner + 1088);

    linked = GetLinkedObjectValue_4DB5C0(self, nullptr);
    int commandOwner = linked ? linked - 4 : 0;

    linked = GetLinkedObjectValue_4DB5C0(self, nullptr);
    owner = linked ? linked - 4 : 0;
    int coverPosition = CSF_AT(int, reinterpret_cast<void*>(owner), 0x630) < 0
        ? owner + 1552
        : owner + 1576;

    auto* commands = reinterpret_cast<S_0041e5f0*>(commandOwner + 1292);
    AiCoverAttackTasks_AppendCoverPositionCommands(commands, nullptr, coverPosition);

    if (static_cast<uint32_t>(commands->field_0008) >=
        static_cast<uint32_t>(commands->field_0004))
        GrowButtonArray(commands, nullptr, commands->field_000c);

    uint32_t index = commands->field_0008;
    auto* entry = reinterpret_cast<uint32_t*>(commands->field_0000) + index;
    commands->field_0008 = index + 1;
    *entry = 31;

    if (addExtraCommands)
    {
        if (static_cast<uint32_t>(commands->field_0008) >=
            static_cast<uint32_t>(commands->field_0004))
            GrowButtonArray(commands, nullptr, commands->field_000c);

        index = commands->field_0008;
        entry = reinterpret_cast<uint32_t*>(commands->field_0000) + index;
        commands->field_0008 = index + 1;
        *entry = 7;

        if (static_cast<uint32_t>(commands->field_0008) >=
            static_cast<uint32_t>(commands->field_0004))
            GrowButtonArray(commands, nullptr, commands->field_000c);

        index = commands->field_0008;
        entry = reinterpret_cast<uint32_t*>(commands->field_0000) + index;
        commands->field_0008 = index + 1;
        *entry = 10;
    }

    if (addRandomDestination)
    {
        void* linkedField = reinterpret_cast<void*>(
            GetLinkedObjectField10(self, nullptr));
        float destination[3] = {
            CSF_AT(float, linkedField, 0x10),
            CSF_AT(float, linkedField, 0x14),
            CSF_AT(float, linkedField, 0x18)
        };

        void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
        destination[0] += static_cast<float>(GetStateValue(state, nullptr, 200) - 100);

        state = reinterpret_cast<void*>(GetGlobalState_50D190());
        destination[2] += static_cast<float>(GetStateValue(state, nullptr, 200) - 100);

        linked = GetLinkedObjectValue_4DB5C0(self, nullptr);
        owner = linked ? linked - 4 : 0;
        void* positionObject = CSF_AT(void*, reinterpret_cast<void*>(owner), 0x2C);
        void* position = reinterpret_cast<void*>(
            GetObjectPositionVector(positionObject, nullptr));

        float deltaX = destination[0] - CSF_AT(float, position, 0);
        float deltaZ = destination[2] - CSF_AT(float, position, 8);
        if (std::fabs(deltaX) > g_QuarterTurnConstant)
        {
            if (std::fabs(deltaZ) > g_QuarterTurnConstant)
                AiCoverAttackTasks_AppendDestinationCommands(
                    commands, nullptr, reinterpret_cast<uint32_t*>(destination));
        }
    }
}

HOOK(0x004C72A0, AiCoverAttackTasks_ConfigureCoverMovementCommands, "auto");
