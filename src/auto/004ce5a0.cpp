#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, AttackTasks_QueueShot, 0x0048C630, (void* self, void* , int a2, float a3, float a4, int a5, int a6));
CSF_FUNC(char, __fastcall, C_0083ac40_SetAttackMode, 0x004CE350, (void* self, void* edx, int a2));

static char __fastcall C_0083ac40_HandleAttackCommand(
    C_0083ac40* self, void* /*edx*/, int commandAddress)
{
    void* command = reinterpret_cast<void*>(commandAddress);
    char result = static_cast<char>(commandAddress);

    switch (CSF_AT(uint32_t, command, 0x00))
    {
    case 0:
        self->field_0018 |= 1u;
        self->field_0078 = CSF_AT(uint32_t, command, 0x04);
        CSF_AT(uint32_t, self, 0x7C) = CSF_AT(uint32_t, command, 0x08);
        CSF_AT(uint32_t, self, 0x80) = CSF_AT(uint32_t, command, 0x0C);
        result = C_0083ac40_SetAttackMode(self, nullptr, 1);
        break;

    case 0xB:
        self->field_0018 |= 1u;
        self->field_008c = CSF_AT(uint32_t, command, 0x04);
        CSF_AT(uint32_t, self, 0x90) = CSF_AT(uint32_t, command, 0x08);
        CSF_AT(uint32_t, self, 0x94) = CSF_AT(uint32_t, command, 0x0C);
        self->field_0098 = CSF_AT(uint32_t, command, 0x10);
        CSF_AT(uint32_t, self, 0x9C) = CSF_AT(uint32_t, command, 0x14);
        self->field_00a0 = CSF_AT(uint32_t, command, 0x18);
        self->field_0088 = CSF_AT(float, command, 0x1C);
        result = C_0083ac40_SetAttackMode(self, nullptr, 2);
        break;

    case 0xE:
    {
        const int shotFlags = CSF_AT(int, command, 0x14);
        const int shotType = CSF_AT(int, command, 0x10);
        const float secondParameter = CSF_AT(float, command, 0x0C);
        const float firstParameter = CSF_AT(float, command, 0x08);
        const int shotTarget = CSF_AT(int, command, 0x04);
        result = static_cast<char>(AttackTasks_QueueShot(
            reinterpret_cast<uint8_t*>(self) + 0x40, nullptr,
            shotTarget, firstParameter, secondParameter, shotType, shotFlags));
        break;
    }

    case 0x26:
        if (self->field_0070 == 1)
        {
            void* owner = reinterpret_cast<void*>(self->field_0014);
            void* task = CSF_AT(void*, owner, 0x30);
            result = static_cast<char>(
                reinterpret_cast<int(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, task, 0)[0xA0 / 4])(task, nullptr));
            self->field_0070 = 6;
            self->field_0018 &= ~1u;
        }
        break;

    default:
        break;
    }

    return result;
}

HOOK(0x004CE5A0, C_0083ac40_HandleAttackCommand, "auto");
