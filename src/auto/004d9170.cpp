#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AttackTasks_SetMode, 0x0047B210, (void* self, void* , int mode));
CSF_FUNC(unsigned int, __fastcall, AttackTasks_QueueShot, 0x0048C630, (void* self, void* edx, int a2, float a3, float a4, int a5, int a6));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObject_PassesFirstCheck, 0x0048E120, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00833908_GetTargetName, 0x0048FE90, (C_00833908* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetStance_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiAutonomous_SetCharacterCombatOption, 0x00492750, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Character_SetActionMode, 0x00492760, (void* self, void* edx, int a2));
CSF_FUNC(char*, __fastcall, Task_GetTargetNameBuffer, 0x004D1510, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResetState, 0x004D16F0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCore_UpdateTargetData, 0x004D1730, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiAutonomous_ResetAdvancedMovement, 0x004D70B0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiAutonomous_SetMovementDestination, 0x004D7DC0, (void* self, void* edx, int a2, int a3, char a4, int a5));
CSF_FUNC(int, __fastcall, AiAutonomous_SetAlternateMovementDestination, 0x004D7E10, (void* self, void* edx, int a2, int a3, char a4, int a5));
CSF_FUNC(int, __fastcall, AiAutonomous_SetTimedMovement, 0x004D7F10, (void* self, void* edx, float a2, char a3));
CSF_FUNC(int, __fastcall, AiAutonomous_SetMovementTarget, 0x004D7F50, (void* self, void* edx, int arg0, int a3));
CSF_FUNC(int, __fastcall, AiAutonomous_SetAlternateMovementTarget, 0x004D7FA0, (void* self, void* edx, int arg0, int a3));
CSF_FUNC(char, __fastcall, AiAutonomous_SetTimedAction, 0x004D8240, (void* self, void* edx, float a2));
CSF_FUNC(char, __fastcall, AiAutonomous_SetObjectActionPosition, 0x004D8260, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, AiAutonomous_SetAlternateObjectActionPosition, 0x004D82B0, (void* self, void* edx, int a3, int a4));
CSF_FUNC(int, __fastcall, AiAutonomous_EndAlternateObjectAction, 0x004D82D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiAutonomous_EndObjectAction, 0x004D82F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiAutonomous_SetScriptedAction, 0x004D8770, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, AiAutonomous_SetIndexedMovementTarget, 0x004D8950, (void* self, void* edx, int arg0, int a3));
CSF_FUNC(int, __fastcall, AiAutonomous_SetAdvancedMovementDestination, 0x004D8B00, (void* self, void* edx, int arg0, int a3, char a4));
CSF_FUNC(int, __fastcall, AiAutonomous_StartItemRefill, 0x004D8B80, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiAutonomous_SetAdvancedAction, 0x004D8BE0, (void* self, void* edx, int arg0));
CSF_FUNC(int, __fastcall, AiAutonomous_SetAdvancedTarget, 0x004D8C50, (void* self, void* edx, int arg0));
CSF_FUNC(int, __fastcall, AiAutonomous_SetAdvancedPositionPair, 0x004D8CC0, (void* self, void* edx, float* arg0, float* a3, int a4));
CSF_FUNC(int, __fastcall, AiAutonomous_SetAdvancedObjectTarget, 0x004D8D30, (void* self, void* edx, void* arg0, unsigned int a3, int a4));
CSF_FUNC(int, __fastcall, AiAutonomous_SetAdvancedPositionAction, 0x004D8DA0, (void* self, void* edx, int arg0, float a3, int a4));
CSF_FUNC(void, __fastcall, AiAutonomous_StopAdvancedMovement, 0x004D8E10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, SceneEntity_Activate, 0x004E51B0, (void* self, void* edx, char a2));
CSF_FUNC(char, __fastcall, AiAutonomous_SetEntityActionFlag, 0x004E5230, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, AiAutonomous_SetEntityIndexedActionFlag, 0x004E5250, (void* self, void* edx, int a2, char a3));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));
CSF_FUNC(char*, __cdecl, AiAutonomous_GetActionStateName, 0x007613A0, (int a1));

static void __fastcall C_0083dbb0_DispatchCommand(
    C_0083dbb0* self, void* /*edx*/, int commandAddress)
{
    auto* command = reinterpret_cast<uint8_t*>(commandAddress);
    bool inactive =
        static_cast<uint8_t>(SceneObject_PassesFirstCheck(
            reinterpret_cast<void*>(self->field_0014), nullptr)) != 0;
    if (!inactive)
        inactive = static_cast<uint8_t>(SceneObj_IsInactive(
            reinterpret_cast<void*>(self->field_0014), nullptr)) != 0;

    if ((self->field_0010 && inactive) ||
        !(reinterpret_cast<C_00833908*>(self->field_0014)->field_0170 & 0x10))
    {
        auto* character = reinterpret_cast<C_00833908*>(self->field_0014);
        if ((character->field_0170 & 0x10) &&
            CSF_AT(int, command, 0) == 29 &&
            !static_cast<uint8_t>(SceneObj_IsInactive(character, nullptr)))
            goto stopCharacter;
        return;
    }

    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0008), 0x288) = 0;

    C_00833908* unsupportedCharacter;
    switch (CSF_AT(int, command, 0))
    {
    case 0:
    {
        if (CSF_AT(int, command, 0x10) == -1)
        {
            void* movement = reinterpret_cast<void*>(CharTask_StartMovement(
                self, nullptr, reinterpret_cast<int>(&self->pad_004c[252])));
            AiAutonomous_SetAdvancedMovementDestination(
                movement, nullptr, commandAddress + 4,
                CSF_AT(uint8_t, command, 0x15), CSF_AT(char, command, 0x16));
        }
        else
        {
            void* movement = reinterpret_cast<void*>(CharTask_StartMovement(
                self, nullptr, reinterpret_cast<int>(&self->pad_004c[68])));
            char option = CSF_AT(char, command, 0x16);
            bool alternate = CSF_AT(uint8_t, command, 0x14) != 0;
            int target = CSF_AT(int, command, 0x10);
            if (alternate)
                AiAutonomous_SetAlternateMovementDestination(
                    movement, nullptr, commandAddress + 4,
                    CSF_AT(uint8_t, command, 0x15), option, target);
            else
                AiAutonomous_SetMovementDestination(
                    movement, nullptr, commandAddress + 4,
                    CSF_AT(uint8_t, command, 0x15), option, target);
        }
        self->field_0318 = CSF_AT(uint32_t, command, 0);
        self->field_031c = CSF_AT(uint32_t, command, 4);
        self->field_0320 = CSF_AT(uint32_t, command, 8);
        self->field_0324 = CSF_AT(uint32_t, command, 12);
        self->field_0328 = CSF_AT(uint32_t, command, 16);
        uint32_t options = CSF_AT(uint32_t, command, 20);
        self->field_0314 = 1;
        std::memcpy(&self->field_032c, &options, sizeof(options));
        return;
    }
    case 2:
    {
        void* movement = reinterpret_cast<void*>(CharTask_StartMovement(
            self, nullptr, reinterpret_cast<int>(&self->pad_004c[128])));
        AiAutonomous_SetMovementTarget(
            movement, nullptr, CSF_AT(int, command, 4), CSF_AT(int, command, 8));
        return;
    }
    case 3:
        self->field_0039 = CSF_AT(uint8_t, command, 4);
        if (self->field_0039)
        {
            void* innerTask = reinterpret_cast<void*>(self->field_0010);
            if (innerTask &&
                reinterpret_cast<int(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, innerTask, 0)[128 / 4])(innerTask, nullptr) == 8)
                CharTask_StartMovement(
                    self, nullptr, reinterpret_cast<int>(&self->pad_004c[8]));
        }
        else
            CharTask_StartMovement(
                self, nullptr, reinterpret_cast<int>(&self->pad_004c[48]));
        return;

    case 4:
        self->field_003a = CSF_AT(uint8_t, command, 4);
        if (self->field_003a)
            CharTask_StartMovement(
                self, nullptr, reinterpret_cast<int>(&self->pad_004c[88]));
        return;

    case 5:
        if (reinterpret_cast<C_00833908*>(self->field_0014)->field_003c)
            SceneEntity_Activate(
                reinterpret_cast<void*>(
                    reinterpret_cast<C_00833908*>(self->field_0014)->field_003c),
                nullptr, CSF_AT(char, command, 4));
        return;

    case 6:
        if (CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0014), 0x1c4) == 1)
            CharTask_StartMovement(
                self, nullptr, reinterpret_cast<int>(&self->pad_004c[8]));
        else
            CharTask_StartMovement(
                self, nullptr, reinterpret_cast<int>(&self->pad_004c[152]));
        return;

    case 7:
        if (!CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0014), 0x1c4))
            CharTask_StartMovement(
                self, nullptr, reinterpret_cast<int>(&self->pad_004c[8]));
        else
            CharTask_StartMovement(
                self, nullptr, reinterpret_cast<int>(&self->pad_004c[172]));
        return;

    case 0xA:
    {
        unsupportedCharacter = reinterpret_cast<C_00833908*>(self->field_0014);
        if (CSF_AT(int, unsupportedCharacter, 0x1d8) != 3)
            goto unsupportedAction;
        void* movement = reinterpret_cast<void*>(CharTask_StartMovement(
            self, nullptr, reinterpret_cast<int>(&self->pad_004c[252])));
        AiAutonomous_SetAdvancedTarget(movement, nullptr, CSF_AT(int, command, 4));
        return;
    }
    case 0xB:
    {
        unsupportedCharacter = reinterpret_cast<C_00833908*>(self->field_0014);
        if (CSF_AT(int, unsupportedCharacter, 0x1d8) != 3)
            goto unsupportedAction;
        void* movement = reinterpret_cast<void*>(CharTask_StartMovement(
            self, nullptr, reinterpret_cast<int>(&self->pad_004c[252])));
        AiAutonomous_SetAdvancedPositionPair(
            movement, nullptr, reinterpret_cast<float*>(command + 4),
            reinterpret_cast<float*>(command + 16), CSF_AT(int, command, 28));
        return;
    }
    case 0xC:
    {
        unsupportedCharacter = reinterpret_cast<C_00833908*>(self->field_0014);
        if (CSF_AT(int, unsupportedCharacter, 0x1d8) != 3)
            goto unsupportedAction;
        void* movement = reinterpret_cast<void*>(CharTask_StartMovement(
            self, nullptr, reinterpret_cast<int>(&self->pad_004c[252])));
        AiAutonomous_SetAdvancedObjectTarget(
            movement, nullptr, CSF_AT(void*, command, 4),
            CSF_AT(unsigned int, command, 8), CSF_AT(int, command, 12));
        return;
    }
    case 0xD:
    {
        unsupportedCharacter = reinterpret_cast<C_00833908*>(self->field_0014);
        if (CSF_AT(int, unsupportedCharacter, 0x1d8) != 3)
            goto unsupportedAction;
        void* movement = reinterpret_cast<void*>(CharTask_StartMovement(
            self, nullptr, reinterpret_cast<int>(&self->pad_004c[252])));
        AiAutonomous_SetAdvancedPositionAction(
            movement, nullptr, commandAddress + 4,
            CSF_AT(float, command, 16), CSF_AT(int, command, 20));
        return;
    }
    case 0xE:
        AttackTasks_QueueShot(
            reinterpret_cast<uint8_t*>(CSF_AT(
                uint32_t, reinterpret_cast<void*>(self->field_0008), 0x308)) + 0x2b8,
            nullptr, CSF_AT(int, command, 4), CSF_AT(float, command, 8),
            CSF_AT(float, command, 12), CSF_AT(int, command, 16),
            CSF_AT(int, command, 20));
        return;

    case 0xF:
    {
        int mode = CSF_AT(int, command, 4);
        if (mode == 0 || mode == 1 || mode == 2)
        {
            auto* character = reinterpret_cast<C_00833908*>(self->field_0014);
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                character->vtbl[0xb4 / 4])(character, nullptr, mode);
        }
        void* controller = CSF_AT(
            void*, reinterpret_cast<void*>(self->field_0008), 20);
        reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int, int)>(
            CSF_AT(void**, controller, 0)[140 / 4])(
                controller, nullptr, 0, 0, -1, 0);
        return;
    }
    case 0x10:
        AttackTasks_SetMode(
            reinterpret_cast<uint8_t*>(CSF_AT(
                uint32_t, reinterpret_cast<void*>(self->field_0008), 0x308)) + 0x2b8,
            nullptr, CSF_AT(int, command, 4));
        return;

    case 0x11:
    {
        void* inventory =
            reinterpret_cast<void*>(
                reinterpret_cast<C_00833908*>(self->field_0014)->field_0044);
        if (reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, inventory, 0)[216 / 4])(inventory, nullptr))
        {
            void* item = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, inventory, 0)[216 / 4])(inventory, nullptr);
            if (GetCurrentItemCount(item, nullptr) < CSF_AT(int, command, 4))
            {
                void* movement = reinterpret_cast<void*>(CharTask_StartMovement(
                    self, nullptr, reinterpret_cast<int>(&self->pad_004c[252])));
                AiAutonomous_StartItemRefill(movement, nullptr);
            }
        }
        return;
    }
    case 0x12:
    {
        auto* target = reinterpret_cast<uint8_t*>(self->field_0308) + 24;
        CSF_AT(uint32_t, target, 4) = CSF_AT(uint32_t, command, 8);
        CSF_AT(uint32_t, target, 8) = CSF_AT(uint32_t, command, 12);
        auto* state = reinterpret_cast<uint8_t*>(self->field_0308);
        if (CSF_AT(int, state, 32) < 0)
        {
            char* buffer = Task_GetTargetNameBuffer(self, nullptr);
            ResetState(buffer, nullptr);
        }
        else
        {
            int targetAddress = reinterpret_cast<int>(state + 24);
            char* buffer = Task_GetTargetNameBuffer(self, nullptr);
            AiCore_UpdateTargetData(buffer, nullptr, targetAddress);
        }
        return;
    }
    case 0x13:
        AiAutonomous_SetCharacterCombatOption(
            reinterpret_cast<void*>(self->field_0014),
            nullptr, CSF_AT(int, command, 4));
        return;

    case 0x14:
    {
        unsupportedCharacter = reinterpret_cast<C_00833908*>(self->field_0014);
        if (CSF_AT(int, unsupportedCharacter, 0x1d8) != 3)
            goto unsupportedAction;
        void* movement = reinterpret_cast<void*>(CharTask_StartMovement(
            self, nullptr, reinterpret_cast<int>(&self->pad_004c[252])));
        AiAutonomous_SetAdvancedAction(movement, nullptr, CSF_AT(int, command, 4));
        return;
    }
    case 0x15:
        CoverAttack_SetStance_492720(
            reinterpret_cast<void*>(self->field_0014),
            nullptr, CSF_AT(int, command, 4));
        return;

    case 0x16:
        Character_SetActionMode(
            reinterpret_cast<void*>(self->field_0014),
            nullptr, CSF_AT(int, command, 4));
        return;

    case 0x17:
    {
        void* movement = reinterpret_cast<void*>(CharTask_StartMovement(
            self, nullptr, reinterpret_cast<int>(&self->pad_004c[212])));
        AiAutonomous_SetTimedAction(movement, nullptr, CSF_AT(float, command, 4));
        return;
    }
    case 0x18:
    {
        void* movement = reinterpret_cast<void*>(CharTask_StartMovement(
            self, nullptr, reinterpret_cast<int>(&self->pad_004c[108])));
        AiAutonomous_SetTimedMovement(
            movement, nullptr, CSF_AT(float, command, 4), CSF_AT(char, command, 8));
        return;
    }
    case 0x19:
    {
        void* movement = reinterpret_cast<void*>(CharTask_StartMovement(
            self, nullptr, reinterpret_cast<int>(&self->pad_004c[128])));
        int targetIndex = CSF_AT(int, command, 12);
        if (targetIndex == -1)
            AiAutonomous_SetAlternateMovementTarget(
                movement, nullptr, CSF_AT(int, command, 4), CSF_AT(int, command, 8));
        else
            AiAutonomous_SetIndexedMovementTarget(
                movement, nullptr, CSF_AT(int, command, 4), targetIndex);
        return;
    }
    case 0x1A:
        if (reinterpret_cast<C_00833908*>(self->field_0014)->field_003c)
        {
            if (CSF_AT(int, command, 4) == 1)
                AiAutonomous_SetEntityActionFlag(
                    reinterpret_cast<void*>(
                        reinterpret_cast<C_00833908*>(self->field_0014)->field_003c),
                    nullptr, CSF_AT(char, command, 8));
            else if (CSF_AT(int, command, 4) == 2)
                AiAutonomous_SetEntityIndexedActionFlag(
                    reinterpret_cast<void*>(
                        reinterpret_cast<C_00833908*>(self->field_0014)->field_003c),
                    nullptr, CSF_AT(int, command, 12), CSF_AT(char, command, 8));
        }
        return;

    case 0x1B:
    {
        void* movement = reinterpret_cast<void*>(CharTask_StartMovement(
            self, nullptr, reinterpret_cast<int>(&self->pad_004c[232])));
        AiAutonomous_SetObjectActionPosition(
            movement, nullptr, commandAddress + 4, CSF_AT(int, command, 16));
        return;
    }
    case 0x1C:
    {
        void* movement = reinterpret_cast<void*>(CharTask_StartMovement(
            self, nullptr, reinterpret_cast<int>(&self->pad_004c[232])));
        AiAutonomous_EndObjectAction(movement, nullptr);
        return;
    }
    case 0x1D:
        goto stopCharacter;

    case 0x20:
    {
        void* movement = reinterpret_cast<void*>(CharTask_StartMovement(
            self, nullptr, reinterpret_cast<int>(&self->pad_004c[232])));
        AiAutonomous_SetAlternateObjectActionPosition(
            movement, nullptr, commandAddress + 4, CSF_AT(int, command, 16));
        return;
    }
    case 0x21:
    {
        void* movement = reinterpret_cast<void*>(CharTask_StartMovement(
            self, nullptr, reinterpret_cast<int>(&self->pad_004c[232])));
        AiAutonomous_EndAlternateObjectAction(movement, nullptr);
        return;
    }
    case 0x23:
    {
        void* movement = reinterpret_cast<void*>(CharTask_StartMovement(
            self, nullptr, reinterpret_cast<int>(&self->pad_004c[608])));
        AiAutonomous_SetScriptedAction(
            movement, nullptr, reinterpret_cast<uint32_t*>(command + 4));
        return;
    }
    case 0x24:
        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_030c), 1704) =
            CSF_AT(uint32_t, command, 4);
        return;

    case 0x25:
        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_030c), 1708) =
            CSF_AT(uint32_t, command, 4);
        return;

    case 0x26:
        AiAutonomous_ResetAdvancedMovement(&self->pad_004c[252], nullptr);
        return;

    case 0x27:
        AiAutonomous_StopAdvancedMovement(&self->pad_004c[252], nullptr);
        return;

    default:
        return;
    }

unsupportedAction:
    C_00833908_GetTargetName(unsupportedCharacter, nullptr);
    AiAutonomous_GetActionStateName(
        CSF_AT(int, reinterpret_cast<void*>(self->field_0014), 0x1d8));
    return;

stopCharacter:
    void* actor = reinterpret_cast<void*>(
        reinterpret_cast<C_00833908*>(self->field_0014)->field_0038);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, actor, 0)[156 / 4])(actor, nullptr);
}

HOOK(0x004D9170, C_0083dbb0_DispatchCommand, "auto");
