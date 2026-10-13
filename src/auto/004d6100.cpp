#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CMenuMpGameMode_GetSceneEntry, 0x0040F790, (CMenuMpGameMode* self, void* , int index));
CSF_FUNC(unsigned int, __fastcall, AttackTasks_QueueShot, 0x0048C630, (void* self, void* , int a2, float a3, float a4, int a5, int a6));
CSF_FUNC(int, __fastcall, ResetState, 0x004D16F0, (void* self, void*));
CSF_FUNC(bool, __fastcall, C_0083c280_EnterAttackState, 0x004D5520, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_0083c280_ResetTask, 0x004D5840, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_0083c280_StartPositionTask, 0x004D5D90, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiCore_GetSceneEntryCount, 0x0052E420, (void* self, void* edx));

// The original dispatches with `cmp eax, 0x26; ja 0x4D6297; movzx eax, byte [0x4D62B4 + eax]; jmp [...]`.
// The shared exit 0x4D6297 returns eax as it is: the command itself when it is above 0x26, otherwise the table
// byte (the case index, 4 for a command with no case of its own) left by the movzx.
static char SwitchExitValue(uint32_t command)
{
    if (command > 0x26)
        return static_cast<char>(command);
    return static_cast<char>(reinterpret_cast<const uint8_t*>(CSF_ADDR_DATA(0x004D62B4))[command]);
}

static char __fastcall C_0083c280_HandleCommand(
    C_0083c280* self, void* /*edx*/, float* arguments)
{
    const uint32_t command = CSF_AT(uint32_t, arguments, 0);
    switch (command)
    {
    case 0:
    {
        auto* scene = reinterpret_cast<C_0082dd78*>(
            CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0014), 0x4C));

        for (int index = 0; index < AiCore_GetSceneEntryCount(scene, nullptr); ++index)
        {
            const int entry = CMenuMpGameMode_GetSceneEntry(
                reinterpret_cast<CMenuMpGameMode*>(scene), nullptr, index);
            if (CSF_AT(uint8_t, reinterpret_cast<void*>(entry), 0x0C))
            {
                ((void(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, scene, 0)[0x4C / 4])(scene, nullptr, index);
            }
        }

        self->field_0018 |= 1u;
        self->field_0078 = CSF_AT(uint32_t, arguments, 0x04);
        self->field_007c = CSF_AT(uint32_t, arguments, 0x08);
        self->field_0080 = CSF_AT(uint32_t, arguments, 0x0C);
        return C_0083c280_StartPositionTask(self, nullptr, 1);
    }

    case 0xB:
    {
        const bool targetable = ((bool(__fastcall*)(void*, void*))
            CSF_AT(void**, self, 0)[0xD8 / 4])(self, nullptr);
        if (!targetable)
            return static_cast<char>(targetable);

        self->field_0018 |= 1u;
        self->field_008c = CSF_AT(uint32_t, arguments, 0x04);
        self->field_0090 = CSF_AT(uint32_t, arguments, 0x08);
        self->field_0094 = CSF_AT(uint32_t, arguments, 0x0C);
        self->field_0098 = CSF_AT(uint32_t, arguments, 0x10);
        self->field_009c = CSF_AT(uint32_t, arguments, 0x14);
        const uint32_t state = self->field_0070;
        self->field_00a0 = CSF_AT(uint32_t, arguments, 0x18);
        self->field_0088 = arguments[7];

        if (state == 1)
        {
            void* task = reinterpret_cast<void*>(
                CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0014), 0x30));
            ((void(__fastcall*)(void*, void*))CSF_AT(void**, task, 0)[0xA0 / 4])(
                task, nullptr);
        }
        else if (state == 3)
        {
            C_0083c280_ResetTask(self, nullptr);
        }
        else if (state == 0)
        {
            void* controller = reinterpret_cast<void*>(
                CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0014), 0x44));
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, controller, 0)[0xBC / 4])(controller, nullptr, 13);
        }

        self->field_0070 = 2;
        return static_cast<char>(C_0083c280_EnterAttackState(self, nullptr));
    }

    case 0xE:
        return static_cast<char>(AttackTasks_QueueShot(
            reinterpret_cast<uint8_t*>(self) + 0x40, nullptr,
            CSF_AT(int, arguments, 0x04),
            arguments[2], arguments[3],
            CSF_AT(int, arguments, 0x10),
            CSF_AT(int, arguments, 0x14)));

    case 0x26:
        if (self->field_0070 == 1)
        {
            void* task = reinterpret_cast<void*>(
                CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0014), 0x30));
            ((void(__fastcall*)(void*, void*))CSF_AT(void**, task, 0)[0xA0 / 4])(
                task, nullptr);

            self->field_0070 = 6;
            const char result = static_cast<char>(ResetState(&self->field_001c, nullptr));
            self->field_0018 &= ~1u;
            return result;
        }
        return SwitchExitValue(command);

    default:
        return SwitchExitValue(command);
    }
}

HOOK(0x004D6100, C_0083c280_HandleCommand, "auto");
