#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, Vehicle_AimAtPosition, 0x0041C960, (void* self, void* , float* position, char interpolate));
CSF_FUNC(int, __fastcall, ResetState, 0x004D16F0, (void* self, void*));
CSF_FUNC(uint32_t*, __fastcall, C_0083c280_ResetTask, 0x004D5840, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_0083c280_CompleteAimTransition, 0x004D5B50, (void* self, void* edx));

static char __fastcall C_0083c280_UpdateAim(C_0083c280* self, void* /*edx*/, float deltaTime)
{
    if (((bool(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0xD8 / 4])
        (self, nullptr))
    {
        const float elapsed = deltaTime + self->field_0084;
        void** trajectoryVtable = CSF_AT(void**, &self->field_00a4, 0);
        void* owner = reinterpret_cast<void*>(self->field_0014);
        void* vehicle = CSF_AT(void*, owner, 0x4C);
        self->field_0084 = elapsed;

        alignas(8) uint8_t positionBytes[0xC];
        float* position = reinterpret_cast<float*>(positionBytes);

        if (elapsed < self->field_0088)
        {
            ((void(__fastcall*)(void*, void*, float*, float))
                trajectoryVtable[0x14 / 4])
                (&self->field_00a4, nullptr, position, elapsed);
            return Vehicle_AimAtPosition(vehicle, nullptr, position, 0);
        }

        ((void(__fastcall*)(void*, void*, float*, float))
            trajectoryVtable[0x14 / 4])
            (&self->field_00a4, nullptr, position, self->field_0088);
        Vehicle_AimAtPosition(vehicle, nullptr, position, 0);

        switch (self->field_0070)
        {
        case 0:
        {
            void* controller = CSF_AT(
                void*, reinterpret_cast<void*>(self->field_0014), 0x44);
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, controller, 0)[0xBC / 4])
                (controller, nullptr, 13);
            break;
        }
        case 1:
        {
            void* task = CSF_AT(
                void*, reinterpret_cast<void*>(self->field_0014), 0x30);
            ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, task, 0)[0xA0 / 4])
                (task, nullptr);
            break;
        }
        }

        self->field_0070 = 4;
        return C_0083c280_CompleteAimTransition(self, nullptr);
    }

    C_0083c280_ResetTask(self, nullptr);
    switch (self->field_0070)
    {
    case 0:
    {
        void* controller = CSF_AT(
            void*, reinterpret_cast<void*>(self->field_0014), 0x44);
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, controller, 0)[0xBC / 4])
            (controller, nullptr, 13);
        break;
    }
    case 1:
    {
        void* task = CSF_AT(
            void*, reinterpret_cast<void*>(self->field_0014), 0x30);
        ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, task, 0)[0xA0 / 4])
            (task, nullptr);
        break;
    }
    case 3:
        C_0083c280_ResetTask(self, nullptr);
        break;
    }

    self->field_0070 = 6;
    const char result = static_cast<char>(ResetState(&self->field_001c, nullptr));
    self->field_0018 &= ~1u;
    return result;
}

HOOK(0x004D5F40, C_0083c280_UpdateAim, "auto");
