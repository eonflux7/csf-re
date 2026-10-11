#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, AttackTasks_QueueShot, 0x0048C630, (void* self, void* edx, int a2, float a3, float a4, int a5, int a6));
CSF_FUNC(int, __fastcall, ResetState, 0x004D16F0, (void* self, void*));
CSF_FUNC(uint8_t, __fastcall, AutonomousTask_ApplyState, 0x004DAAE0, (void* self, void* edx));
CSF_FUNC(uint8_t, __fastcall, AutonomousTask_SetMode, 0x004DAC20, (void* self, void* edx, int a2));

static uint8_t __fastcall C_0083dd58_HandleMessage(C_0083dd58* self, void* /*edx*/, uint8_t* msg)
{
    uint8_t result = (uint8_t)(uintptr_t)msg;

    switch (CSF_AT(uint32_t, msg, 0))
    {
    case 0:
        self->field_0018 |= 1u;
        self->field_0084 = CSF_AT(uint32_t, msg, 4);
        CSF_AT(uint32_t, self, 0x88) = CSF_AT(uint32_t, msg, 8);
        CSF_AT(uint32_t, self, 0x8C) = CSF_AT(uint32_t, msg, 12);
        result = AutonomousTask_SetMode(self, nullptr, 1);
        break;

    case 0xE:
        result = (uint8_t)AttackTasks_QueueShot(
            (uint8_t*)self + 0x40, nullptr,
            CSF_AT(int, msg, 4),
            CSF_AT(float, msg, 8),
            CSF_AT(float, msg, 12),
            CSF_AT(int, msg, 16),
            CSF_AT(int, msg, 20));
        break;

    case 0x1E:
        self->field_0018 |= 1u;
        self->field_0078 = CSF_AT(uint32_t, msg, 4);
        CSF_AT(uint32_t, self, 0x7C) = CSF_AT(uint32_t, msg, 8);
        CSF_AT(uint32_t, self, 0x80) = CSF_AT(uint32_t, msg, 12);
        result = AutonomousTask_SetMode(self, nullptr, 2);
        break;

    case 0x1F:
    {
        self->field_0018 |= 1u;
        if (!self->field_0070)
        {
            uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0014);
            void** obj = CSF_AT(void**, owner, 0x44);
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, obj, 0)[0xBC / 4])(obj, nullptr, 12);
        }
        self->field_0070 = 3;
        result = AutonomousTask_ApplyState(self, nullptr);
        break;
    }

    case 0x26:
        if (self->field_0070 == 1)
        {
            {
                uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0014);
                void** obj = CSF_AT(void**, owner, 0x30);
                ((void(__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[0xA0 / 4])(obj, nullptr);
            }
            if (!self->field_0070)
            {
                uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0014);
                void** obj = CSF_AT(void**, owner, 0x44);
                ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, obj, 0)[0xBC / 4])(obj, nullptr, 12);
            }
            self->field_0070 = 5;
            result = (uint8_t)ResetState((uint8_t*)self + 0x1C, nullptr);
            self->field_0018 &= ~1u;
        }
        break;

    default:
        return result;
    }
    return result;
}

HOOK(0x004DAF80, C_0083dd58_HandleMessage, "auto");
