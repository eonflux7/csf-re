#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_0082dfd0_ApplyMovementVector, 0x00413DE0,
         (void* self, void* edx, float* a2, float a3));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430,
         (void* self, void* edx));
CSF_FUNC(double, __fastcall, CPlayerCharacter_GetContextHeightOffset, 0x004954A0,
         (void* self, void* edx));
CSF_FUNC(float*, __fastcall, S_00526d40_ProjectPointToPlane, 0x006AD830,
         (S_00526d40* self, void* , float* a2, float* a3));

CSF_GLOBAL(float, g_AxisY, 0x008A7D78);
CSF_GLOBAL(float, g_AxisY_Y, 0x008A7D7C);
CSF_GLOBAL(float, g_AxisY_Z, 0x008A7D80);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static int __fastcall C_0082dfd0_ResetAndApplyContextMovement(
    C_0082dfd0* self, void* /*edx*/, float amount,
    int /*unused*/, int* inputVector)
{
    self->field_008c = 0;
    self->field_008a = 0;
    self->field_00d4 = 0.0f;
    self->field_0084 = 0.0f;

    float movement[3];
    std::memcpy(movement, inputVector, sizeof(movement));

    const double height = CPlayerCharacter_GetContextHeightOffset(
        reinterpret_cast<void*>(self->field_0004), nullptr);

    if (movement[1] > height)
    {
        float planePoint[3] = {0.0f, static_cast<float>(height), 0.0f};
        alignas(4) std::uint8_t planeStorage[0x2C] = {};

        CSF_AT(float, planeStorage, 0x00) = 0.0f;
        CSF_AT(float, planeStorage, 0x04) = planePoint[1];
        CSF_AT(float, planeStorage, 0x08) = 0.0f;
        CSF_AT(float, planeStorage, 0x0C) = g_AxisY;
        CSF_AT(float, planeStorage, 0x10) = g_AxisY_Y;
        CSF_AT(float, planeStorage, 0x14) = g_AxisY_Z;
        CSF_AT(std::uint8_t, planeStorage, 0x28) = 1;

        float* projected = S_00526d40_ProjectPointToPlane(
            reinterpret_cast<S_00526d40*>(planeStorage), nullptr,
            planePoint, reinterpret_cast<float*>(inputVector));

        movement[0] = projected[0];
        movement[1] = projected[1];
        movement[2] = projected[2];
    }

    if (movement[0] != g_ZeroVectorX ||
        movement[1] != g_ZeroVectorY ||
        movement[2] != g_ZeroVectorZ)
    {
        C_0082dfd0_ApplyMovementVector(self, nullptr, movement, amount);

        std::memcpy(&self->field_0024, &g_ZeroVectorX, sizeof(float));
        CSF_AT(float, self, 0x28) = g_ZeroVectorY;
        CSF_AT(float, self, 0x2C) = g_ZeroVectorZ;
    }

    const int position = GetObjectPositionVector(
        reinterpret_cast<void*>(self->field_0004), nullptr);
    self->field_0134 = CSF_AT(
        float, reinterpret_cast<void*>(static_cast<std::uintptr_t>(position)), 0x04);
    return position;
}

HOOK(0x004154D0, C_0082dfd0_ResetAndApplyContextMovement, "auto");
