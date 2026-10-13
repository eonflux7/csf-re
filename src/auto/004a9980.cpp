#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, AttackTasks_QueueShot, 0x0048C630, (void* self, void* , int a2, float a3, float a4, int a5, int a6));
CSF_FUNC(bool, __fastcall, C_00835228_ProcessAttackState, 0x004A8DE0, (void* self, void* edx));

static char __fastcall C_00835228_HandleAttackRequest(
    C_00835228* self, void* /*edx*/, float* request)
{
    const uint32_t requestType = CSF_AT(uint32_t, request, 0);

    switch (requestType)
    {
    case 11:
    {
        const bool accepted =
            reinterpret_cast<bool (__fastcall*)(void*, void*)>(
                CSF_AT(void**, self, 0)[0xD8 / 4])(self, nullptr);
        if (!accepted)
            return static_cast<char>(accepted);

        self->field_0018 |= 1u;
        self->field_0080 = CSF_AT(uint32_t, request, 0x04);
        CSF_AT(uint32_t, self, 0x84) = CSF_AT(uint32_t, request, 0x08);
        CSF_AT(uint32_t, self, 0x88) = CSF_AT(uint32_t, request, 0x0C);
        self->field_008c = CSF_AT(uint32_t, request, 0x10);
        self->field_0090 = CSF_AT(uint32_t, request, 0x14);

        const uint32_t previousState = self->field_0070;
        CSF_AT(uint32_t, self, 0x94) = CSF_AT(uint32_t, request, 0x18);
        self->field_007c = request[7];

        if (!previousState)
        {
            void* owner = reinterpret_cast<void*>(self->field_0014);
            void* controller = CSF_AT(void*, owner, 0x44);
            reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, controller, 0)[0xA0 / 4])(controller, nullptr);
        }

        self->field_0070 = 1;
        return static_cast<char>(C_00835228_ProcessAttackState(self, nullptr));
    }

    case 14:
        return static_cast<char>(AttackTasks_QueueShot(
            reinterpret_cast<uint8_t*>(self) + 0x40, nullptr,
            CSF_AT(int, request, 0x04),
            request[2], request[3],
            CSF_AT(int, request, 0x10),
            CSF_AT(int, request, 0x14)));

    case 25:
    {
        self->field_0018 |= 1u;
        void* owner = reinterpret_cast<void*>(self->field_0014);
        reinterpret_cast<void (__fastcall*)(void*, void*, int, int, int, int)>(
            CSF_AT(void**, owner, 0)[0x90 / 4])(
                owner, nullptr,
                CSF_AT(int, request, 0x04),
                CSF_AT(int, request, 0x08), -1, 0);

        uint32_t result = self->field_0070;
        if (!result)
        {
            owner = reinterpret_cast<void*>(self->field_0014);
            void* controller = CSF_AT(void*, owner, 0x44);
            result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, controller, 0)[0xA0 / 4])(controller, nullptr);
        }

        self->field_0070 = 5;
        return static_cast<char>(result);
    }

    default:
        return static_cast<char>(requestType);
    }
}

HOOK(0x004A9980, C_00835228_HandleAttackRequest, "auto");
