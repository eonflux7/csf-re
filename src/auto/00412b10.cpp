#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, C_0082dfd0_AdjustGroundMovement, 0x00411930,
         (void* self, void* edx, float* a2, int a3));
CSF_FUNC(void, __fastcall, C_0082dfd0_AdjustMovement, 0x004121B0,
         (void* self, void* edx, void* a2, int a3));
CSF_FUNC(uint32_t*, __fastcall, S_00526d40_ConvertDirection, 0x006ABA10,
         (S_00526d40* self, void* , uint32_t* output));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static uint32_t FloatToBits(float value)
{
    uint32_t bits;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

static float BitsToFloat(uint32_t bits)
{
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

static void __fastcall C_0082dfd0_ComputeMovement(
    C_0082dfd0* self, void* /*edx*/,
    S_00526d40* output, float* direction, float scale)
{
    output[0].field_0000 = FloatToBits(g_ZeroVectorX);
    output[1].field_0000 = FloatToBits(g_ZeroVectorY);
    output[2].field_0000 = FloatToBits(g_ZeroVectorZ);

    uint32_t normalized[3];
    float forward[6];
    float lateral[6];
    float movement[3];

    const float forwardAmount = self->field_00f0 - self->field_00f4;
    lateral[3] = g_ZeroVectorX;
    lateral[4] = g_ZeroVectorY;
    lateral[5] = g_ZeroVectorZ;

    forward[0] = forwardAmount * direction[0];
    lateral[0] = direction[2];
    lateral[1] = 0.0f;
    forward[1] = forwardAmount * direction[1];
    forward[2] = forwardAmount * direction[2];

    const float lateralAmount = self->field_00fc - self->field_00f8;
    lateral[2] = -direction[0];

    if (lateral[2] * lateral[2] + lateral[0] * lateral[0] > g_fZero)
    {
        uint32_t* unit = S_00526d40_ConvertDirection(
            reinterpret_cast<S_00526d40*>(lateral), nullptr, normalized);
        forward[3] = lateralAmount * BitsToFloat(unit[0]);
        lateral[3] = forward[3];
        forward[4] = lateralAmount * BitsToFloat(unit[1]);
        lateral[4] = forward[4];
        forward[5] = lateralAmount * BitsToFloat(unit[2]);
        lateral[5] = forward[5];
    }

    movement[0] = lateral[3] + forward[0];
    movement[1] = lateral[4] + forward[1];
    movement[2] = lateral[5] + forward[2];

    if (forwardAmount > g_fZero)
    {
        if (self->field_0110 * self->field_00e0 <
            __builtin_sqrtf(movement[2] * movement[2] +
                            movement[1] * movement[1] +
                            movement[0] * movement[0]))
        {
            uint32_t* unit = S_00526d40_ConvertDirection(
                reinterpret_cast<S_00526d40*>(movement), nullptr, normalized);
            const float limit = self->field_0110 * self->field_00e0;
            forward[0] = limit * BitsToFloat(unit[0]);
            forward[1] = limit * BitsToFloat(unit[1]);
            forward[2] = limit * BitsToFloat(unit[2]);
            movement[0] = forward[0];
            movement[1] = forward[1];
            movement[2] = forward[2];
        }
    }
    else if (forwardAmount < g_fZero)
    {
        if (self->field_00e4 * self->field_0110 <
            __builtin_sqrtf(movement[2] * movement[2] +
                            movement[1] * movement[1] +
                            movement[0] * movement[0]))
        {
            uint32_t* unit = S_00526d40_ConvertDirection(
                reinterpret_cast<S_00526d40*>(movement), nullptr, normalized);
            const float limit = self->field_00e4 * self->field_0110;
            forward[0] = limit * BitsToFloat(unit[0]);
            forward[1] = limit * BitsToFloat(unit[1]);
            forward[2] = limit * BitsToFloat(unit[2]);
            movement[0] = forward[0];
            movement[1] = forward[1];
            movement[2] = forward[2];
        }
    }

    output[0].field_0000 = FloatToBits(movement[0]);
    output[1].field_0000 = FloatToBits(movement[1]);
    output[2].field_0000 = FloatToBits(movement[2]);

    C_0082dfd0_AdjustMovement(
        self, nullptr, output,
        static_cast<int>(reinterpret_cast<intptr_t>(direction)));

    if (!self->field_0089)
    {
        void* owner = reinterpret_cast<void*>(self->field_0004);
        if (CSF_AT(uint32_t, owner, 0x50) ||
            (CSF_AT(uint32_t, owner, 0x2E0) & 0x10000) == 0)
        {
            output[1].field_0000 = FloatToBits(
                self->field_0084 + BitsToFloat(output[1].field_0000));
        }
        else
        {
            C_0082dfd0_AdjustGroundMovement(
                self, nullptr, reinterpret_cast<float*>(output),
                static_cast<int>(reinterpret_cast<intptr_t>(direction)));
        }
    }

    if (self->field_0090 != g_ZeroVectorX ||
        self->field_0094 != g_ZeroVectorY ||
        self->field_0098 != g_ZeroVectorZ)
    {
        if (self->field_009c * self->field_009c +
                self->field_00a4 * self->field_00a4 > g_fZero &&
            BitsToFloat(output[0].field_0000) *
                    BitsToFloat(output[0].field_0000) +
                BitsToFloat(output[2].field_0000) *
                    BitsToFloat(output[2].field_0000) > g_fZero)
        {
            uint32_t* previousUnit = S_00526d40_ConvertDirection(
                reinterpret_cast<S_00526d40*>(&self->field_009c),
                nullptr, normalized);
            uint32_t* movementUnit = S_00526d40_ConvertDirection(
                output, nullptr, reinterpret_cast<uint32_t*>(forward));

            if (BitsToFloat(previousUnit[1]) * BitsToFloat(movementUnit[1]) +
                BitsToFloat(previousUnit[0]) * BitsToFloat(movementUnit[0]) +
                BitsToFloat(previousUnit[2]) * BitsToFloat(movementUnit[2]) <
                g_fZero)
            {
                if (BitsToFloat(output[0].field_0000) * self->field_009c < g_fZero)
                    output[0].field_0000 = 0;
                if (self->field_00a4 * BitsToFloat(output[2].field_0000) < g_fZero)
                    output[2].field_0000 = 0;
            }
        }

        const float timeStep = CSF_AT(
            float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x930);
        const float externalX = timeStep * self->field_0090;
        forward[1] = timeStep * self->field_0094;
        forward[2] = timeStep * self->field_0098;

        output[0].field_0000 = FloatToBits(
            externalX + BitsToFloat(output[0].field_0000));
        output[1].field_0000 = FloatToBits(
            forward[1] + BitsToFloat(output[1].field_0000));
        output[2].field_0000 = FloatToBits(
            forward[2] + BitsToFloat(output[2].field_0000));
    }

    output[0].field_0000 = FloatToBits(scale * BitsToFloat(output[0].field_0000));
    output[1].field_0000 = FloatToBits(scale * BitsToFloat(output[1].field_0000));
    output[2].field_0000 = FloatToBits(scale * BitsToFloat(output[2].field_0000));
}

HOOK(0x00412B10, C_0082dfd0_ComputeMovement, "auto");
