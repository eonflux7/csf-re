#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, C_0082dfd0_GetPosition, 0x00411EF0, (void* self, void* edx, float* a2));
CSF_FUNC(char, __fastcall, C_0082dfd0_ApplyMovement, 0x00413DE0, (void* self, void* edx, float* a2, float a3));
CSF_FUNC(uint32_t*, __fastcall, S_00526d40_ConvertDirection, 0x006ABA10, (S_00526d40* self, void* , uint32_t* output));
CSF_FUNC(float*, __fastcall, S_00526d40_ProjectPointToPlane, 0x006AD830, (S_00526d40* self, void* , float* a2, float* a3));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_MovementAcceleration, 0x0082E074);

static float FloatFromBits(uint32_t bits)
{
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

static int __fastcall C_0082dfd0_UpdateMovement(
    C_0082dfd0* self, void* /*edx*/, float deltaTime,
    int* requestedState, int movementAddress)
{
    const auto* input = reinterpret_cast<const float*>(movementAddress);
    float movement[3] = {input[0], input[1], input[2]};

    if (self->field_008a)
    {
        float position[3] = {};
        C_0082dfd0_GetPosition(self, nullptr, position);

        const int settingsAddress = GetGlobalValue_8BC1E0();
        auto* plane = reinterpret_cast<S_00526d40*>(&self->field_00a8);
        float projectedStart[6] = {};
        float projectedEnd[3] = {};
        float direction[3] = {};

        if (self->field_00d8 <
            CSF_AT(float, reinterpret_cast<void*>(settingsAddress), 0xA58))
        {
            const float length = std::sqrt(
                movement[1] * movement[1] +
                movement[2] * movement[2] +
                movement[0] * movement[0]);

            if (length > g_fZero)
            {
                S_00526d40_ProjectPointToPlane(
                    plane, nullptr, projectedStart, position);

                direction[0] = position[0] + movement[0];
                direction[1] = position[1] + movement[1];
                direction[2] = position[2] + movement[2];
                S_00526d40_ProjectPointToPlane(
                    plane, nullptr, projectedEnd, direction);

                direction[0] = projectedEnd[0] - projectedStart[0];
                direction[1] = projectedEnd[1] - projectedStart[1];
                direction[2] = projectedEnd[2] - projectedStart[2];

                uint32_t converted[3] = {};
                const uint32_t* normalized = S_00526d40_ConvertDirection(
                    reinterpret_cast<S_00526d40*>(direction), nullptr, converted);
                movement[0] = length * FloatFromBits(normalized[0]);
                movement[1] = length * FloatFromBits(normalized[1]);
                movement[2] = length * FloatFromBits(normalized[2]);
            }
        }
        else
        {
            const float directionX = FloatFromBits(self->field_00b4);
            const float directionY = FloatFromBits(self->field_00b8);
            const float directionZ = FloatFromBits(self->field_00bc);
            const float speed = self->field_00d4;

            movement[0] += directionX * speed * deltaTime;
            movement[1] += speed * (directionY * g_MissingMetricValue) * deltaTime;
            movement[2] += directionZ * speed * deltaTime;
            self->field_00d4 =
                deltaTime * g_MovementAcceleration + self->field_00d4;

            S_00526d40_ProjectPointToPlane(
                plane, nullptr, direction, position);
            projectedStart[0] = position[0] + movement[0];
            projectedStart[1] = position[1] + movement[1];
            projectedStart[2] = position[2] + movement[2];
            S_00526d40_ProjectPointToPlane(
                plane, nullptr, projectedEnd, projectedStart);

            movement[0] = projectedEnd[0] - direction[0];
            movement[1] = projectedEnd[1] - direction[1];
            movement[2] = projectedEnd[2] - direction[2];
        }
    }

    const uint8_t forcedUpdate = self->field_008e;
    self->field_008e = 0;
    int result = movement[2] != g_fZero;

    if (static_cast<uint8_t>((movement[0] != g_fZero) | forcedUpdate) |
        static_cast<uint8_t>(result | self->field_008a |
                             self->field_008d | (self->field_0088 == 0)))
    {
        C_0082dfd0_ApplyMovement(self, nullptr, movement, deltaTime);
        result = requestedState[0];
        uint32_t* storedState = &self->field_0024;
        storedState[0] = requestedState[0];
        storedState[1] = requestedState[1];
        storedState[2] = requestedState[2];
    }
    return result;
}

HOOK(0x00415220, C_0082dfd0_UpdateMovement, "auto");
