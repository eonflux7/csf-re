#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_0082dfd0_BeginMovementStep, 0x00411A30,
         (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, C_0082dfd0_FinishMovementStep, 0x00411B10,
         (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, C_0082dfd0_UpdateMovementState, 0x00411C00,
         (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, C_0082dfd0_ReadMovementVector, 0x00412130,
         (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void, __fastcall, C_0082dfd0_CalculateMovementAdjustment, 0x00412B10,
         (void* self, void* edx, void* a2, float* a3, float a4));
CSF_FUNC(int, __fastcall, C_0082dfd0_ApplyNormalMovement, 0x00415220,
         (void* self, void* edx, float a2, int* a3, int a4));
CSF_FUNC(int, __fastcall, C_0082dfd0_ApplySpecialMovement, 0x004154D0,
         (void* self, void* edx, float a2, int a3, int* a4));
CSF_FUNC(int, __fastcall, C_0082dfd0_ApplyAlternateMovement, 0x00415600,
         (void* self, void* edx, float a2, uint32_t* a3, float* a4));

static void __fastcall C_0082dfd0_UpdateMovement(
    C_0082dfd0* self, void* /*edx*/, int stepBits)
{
    float step;
    std::memcpy(&step, &stepBits, sizeof(step));

    C_0082dfd0_BeginMovementStep(self, nullptr, step);

    float movement[3] = {};
    C_0082dfd0_ReadMovementVector(
        self, nullptr, reinterpret_cast<uint32_t*>(movement));

    float adjustment[3] = {};
    C_0082dfd0_CalculateMovementAdjustment(
        self, nullptr, adjustment, movement, step);

    if (self->field_0089)
    {
        C_0082dfd0_ApplyAlternateMovement(
            self, nullptr, step,
            reinterpret_cast<uint32_t*>(movement), adjustment);
    }
    else
    {
        void* owner = reinterpret_cast<void*>(self->field_0004);
        if (CSF_AT(uint32_t, owner, 0x50) ||
            (CSF_AT(uint32_t, owner, 0x2E0) & 0x10000) == 0)
        {
            C_0082dfd0_ApplyNormalMovement(
                self, nullptr, step, reinterpret_cast<int*>(movement),
                reinterpret_cast<int>(adjustment));
        }
        else
        {
            C_0082dfd0_ApplySpecialMovement(
                self, nullptr, step, reinterpret_cast<int>(movement),
                reinterpret_cast<int*>(adjustment));
        }
    }

    C_0082dfd0_FinishMovementStep(self, nullptr, stepBits);
    C_0082dfd0_UpdateMovementState(self, nullptr, step);

    if (!self->field_008c)
    {
        self->field_010c = 0;
        self->field_010d = 0;
        self->field_010e = 0;
        self->field_010f = 0;
    }
}

HOOK(0x00415770, C_0082dfd0_UpdateMovement, "auto");
