#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_FUNC(int, __fastcall, GrowTransitionBuffer, 0x0040AD70,
         (void* self, void* edx, int a2));

static float* __stdcall AppendFloatTransition(void* state, float* transition)
{
    float target;
    if (CSF_AT(std::uint32_t, state, 0x14) != 0)
    {
        float total = g_fZero;
        float* entry = CSF_AT(float*, state, 0x0C);
        std::uint32_t remaining = CSF_AT(std::uint32_t, state, 0x14);
        do
        {
            total += *entry;
            entry += 4;
            --remaining;
        } while (remaining != 0);

        target = total + transition[0];
        CSF_AT(std::uint32_t, state, 0x14) = 0;
    }
    else
    {
        target = transition[0];
    }

    float speed;
    if ((target - CSF_AT(float, state, 0x04)) * transition[1] < g_fZero)
        speed = -transition[1];
    else
        speed = transition[1];

    float acceleration;
    float initialSpeed;
    if (transition[2] == g_fZero)
    {
        initialSpeed = speed;
        acceleration = 0.0f;
    }
    else
    {
        if (CSF_AT(std::int32_t, transition, 0x0C) == -1082130432)
            initialSpeed = CSF_AT(float, state, 0x00);
        else
            initialSpeed = transition[3];

        if ((speed - initialSpeed) * transition[2] < g_fZero)
            acceleration = -transition[2];
        else
            acceleration = transition[2];
    }

    CSF_AT(float, state, 0x00) = initialSpeed;
    CSF_AT(float, state, 0x08) = target;

    if (CSF_AT(std::uint32_t, state, 0x14) >=
        CSF_AT(std::uint32_t, state, 0x10))
    {
        GrowTransitionBuffer(
            static_cast<std::uint8_t*>(state) + 0x0C,
            nullptr,
            CSF_AT(int, state, 0x18));
    }

    const std::uint32_t index = CSF_AT(std::uint32_t, state, 0x14);
    float* result = reinterpret_cast<float*>(
        CSF_AT(std::uint8_t*, state, 0x0C) + 16 * index);
    CSF_AT(std::uint32_t, state, 0x14) = index + 1;

    result[0] = target;
    result[1] = speed;
    result[2] = acceleration;
    result[3] = initialSpeed;
    return result;
}

HOOK(0x0040B130, AppendFloatTransition, "auto");
