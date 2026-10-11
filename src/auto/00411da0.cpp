#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static float* __fastcall C_0082dfd0_SetOrAddVectors(
    C_0082dfd0* self, void* /*edx*/,
    float* firstVector, float* secondVector, char additive)
{
    if (additive)
    {
        self->field_0090 = self->field_0090 + firstVector[0];
        self->field_0094 = firstVector[1] + self->field_0094;
        self->field_0098 = firstVector[2] + self->field_0098;
        self->field_009c = self->field_009c + secondVector[0];
        self->field_00a0 = secondVector[1] + self->field_00a0;
        self->field_00a4 = secondVector[2] + self->field_00a4;
        return secondVector;
    }

    std::memcpy(&self->field_0090, firstVector, sizeof(float));
    std::memcpy(&self->field_0094, firstVector + 1, sizeof(float));
    std::memcpy(&self->field_0098, firstVector + 2, sizeof(float));
    std::memcpy(&self->field_009c, secondVector, sizeof(float));
    std::memcpy(&self->field_00a0, secondVector + 1, sizeof(float));

    std::uint32_t lastComponentBits;
    std::memcpy(&lastComponentBits, secondVector + 2, sizeof(lastComponentBits));
    std::memcpy(&self->field_00a4, &lastComponentBits, sizeof(lastComponentBits));
    return reinterpret_cast<float*>(static_cast<std::uintptr_t>(lastComponentBits));
}

HOOK(0x00411DA0, C_0082dfd0_SetOrAddVectors, "auto");
