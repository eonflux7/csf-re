#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, Vehicle_FindOrCreateByKey, 0x004087D0,
         (void* self, void* edx, int a3));
CSF_FUNC(int, __fastcall, Vehicle_InitRatioVec4, 0x0040A5E0,
         (void* self, void* edx, float a2, float a3));
CSF_FUNC(float*, __fastcall, Vehicle_ApplyTransform, 0x0040B570,
         (void* self, void* edx, int a2, float* a3));
CSF_FUNC(int, __fastcall, Vehicle_UpdateSteeringInputs, 0x00419650,
         (void* self, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(double, g_SteeringAngleNumerator, 0x0082E278);

static void __fastcall C_0082e350_UpdateSteeringAngle(
    C_0082e350* self, void* /*edx*/)
{
    Vehicle_UpdateSteeringInputs(self, nullptr);

    float direction;
    std::memcpy(&direction, &self->field_0028, sizeof(direction));

    void* owner = reinterpret_cast<void*>(self->field_0004);
    void* steeringState = CSF_AT(void*, owner, 0x30);
    if (CSF_AT(uint8_t, steeringState, 0x74))
        direction = -direction;

    // The fifth float shares the original frame's target-angle storage.
    float rotation[5];
    rotation[4] = 0.0f;

    if (direction != g_fZero)
    {
        float radius;
        std::memcpy(&radius, &self->field_002c, sizeof(radius));
        rotation[4] = static_cast<float>(
            std::atan2(g_SteeringAngleNumerator, static_cast<double>(radius)));
        if (!(direction > g_fZero))
            rotation[4] = -rotation[4];
    }

    if (self->field_008c != rotation[4])
    {
        const float angleDelta = rotation[4] - self->field_008c;
        rotation[0] = 0.0f;
        rotation[1] = 0.0f;
        rotation[2] = 0.0f;
        rotation[3] = -1.0f;

        Vehicle_InitRatioVec4(rotation, nullptr, angleDelta, 0.30000001f);
        void* transform = Vehicle_FindOrCreateByKey(
            reinterpret_cast<void*>(self->field_0030), nullptr, 0);
        Vehicle_ApplyTransform(transform, nullptr, 1, rotation);
        self->field_008c = rotation[4];
    }
}

HOOK(0x0041BFD0, C_0082e350_UpdateSteeringAngle, "auto");
