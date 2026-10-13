#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, AiSearchOrders_AimControllerAtPosition, 0x0040CA40, (void* self, void* edx, float* arg0, char a4));
CSF_FUNC(int, __fastcall, C_00835228_ApplyTrajectoryPosition, 0x004A9030, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, C_00835228_FinishTrajectory, 0x004A9340, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResetState, 0x004D16F0, (void* self, void*));
CSF_FUNC(uint32_t*, __fastcall, C_0083c280_ResetTask, 0x004D5840, (void* self, void* edx));

static int __fastcall C_00835228_UpdateAimTrajectory(
    C_00835228* self, void* /*edx*/, float deltaTime)
{
    alignas(8) uint8_t positionBytes[0xC];
    auto* position = reinterpret_cast<float*>(positionBytes);

    if (((bool(__fastcall*)(void*, void*))self->vtbl[0xD8 / 4])(
            self, nullptr))
    {
        const float elapsed = deltaTime + self->field_0078;
        void* owner = reinterpret_cast<void*>(self->field_0014);
        void* aimController = CSF_AT(void*, owner, 0x4C);
        void* trajectory = reinterpret_cast<uint8_t*>(self) + 0x9C;
        self->field_0078 = elapsed;

        // The original also takes the completion branch for unordered comparisons.
        if (!(elapsed < self->field_007c))
        {
            ((void(__fastcall*)(void*, void*, float*, float))
                CSF_AT(void**, trajectory, 0)[0x14 / 4])(
                    trajectory, nullptr, position, self->field_007c);
            AiSearchOrders_AimControllerAtPosition(
                aimController, nullptr, position, 0);
            C_00835228_ApplyTrajectoryPosition(
                self, nullptr, reinterpret_cast<uint32_t*>(positionBytes));

            if (!self->field_0070)
            {
                void* currentOwner = reinterpret_cast<void*>(self->field_0014);
                void* controller = CSF_AT(void*, currentOwner, 0x44);
                ((void(__fastcall*)(void*, void*))
                    CSF_AT(void**, controller, 0)[0xA0 / 4])(
                        controller, nullptr);
            }

            self->field_0070 = 3;
            return C_00835228_FinishTrajectory(self, nullptr);
        }

        ((void(__fastcall*)(void*, void*, float*, float))
            CSF_AT(void**, trajectory, 0)[0x14 / 4])(
                trajectory, nullptr, position, elapsed);
        AiSearchOrders_AimControllerAtPosition(
            aimController, nullptr, position, 0);
        return C_00835228_ApplyTrajectoryPosition(
            self, nullptr, reinterpret_cast<uint32_t*>(positionBytes));
    }

    C_0083c280_ResetTask(self, nullptr);
    if (!self->field_0070)
    {
        void* owner = reinterpret_cast<void*>(self->field_0014);
        void* controller = CSF_AT(void*, owner, 0x44);
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, controller, 0)[0xA0 / 4])(
                controller, nullptr);
    }

    self->field_0070 = 4;
    const int result = ResetState(&self->field_001c, nullptr);
    self->field_0018 &= ~1u;
    return result;
}

HOOK(0x004A9820, C_00835228_UpdateAimTrajectory, "auto");
