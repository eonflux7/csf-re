#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_MessageBoxSizeScale, 0x00831430);
CSF_GLOBAL(float, g_MotionRandomScale, 0x00840540);
CSF_GLOBAL(float, g_MotionTwoPi, 0x00846128);

static double __fastcall CharacterModel_EvaluateMotionChannel(
    void* self, void* /*edx*/, std::int32_t* seed)
{
    double result;
    do
    {
        double wave;
        if (CSF_AT(std::uint8_t, self, 0x01))
        {
            const double phase = CSF_AT(float, self, 0x0C);
            CSF_AT(std::uint8_t, self, 0x01) = 0;
            wave = std::sin(phase * g_MotionTwoPi);
        }
        else
        {
            std::int32_t state = *seed;
            std::int32_t next =
                48271 * (state % 44488) - 3399 * (state / 44488);
            *seed = next;
            if (next <= 0)
                *seed = next + 0x7FFFFFFF;

            CSF_AT(float, self, 0x0C) =
                static_cast<float>(static_cast<double>(*seed) * g_MotionRandomScale);

            state = *seed;
            next = 48271 * (state % 44488) - 3399 * (state / 44488);
            *seed = next;
            if (next <= 0)
                *seed = next + 0x7FFFFFFF;

            const double randomValue = static_cast<double>(*seed);
            CSF_AT(std::uint8_t, self, 0x01) = 1;
            const double uniform = randomValue * g_MotionRandomScale;
            CSF_AT(float, self, 0x10) = static_cast<float>(uniform);
            CSF_AT(float, self, 0x14) = static_cast<float>(
                std::sqrt(
                    (std::log2(g_TransitionOne - uniform) *
                     0.6931471805599453094) *
                    g_MessageBoxSizeScale));

            wave = std::cos(
                static_cast<double>(CSF_AT(float, self, 0x0C)) * g_MotionTwoPi);
        }

        result = wave *
                     (static_cast<double>(CSF_AT(float, self, 0x08)) *
                      CSF_AT(float, self, 0x14)) +
                 CSF_AT(float, self, 0x04);
    }
    while (result > g_TransitionOne || result < g_fZero);

    return result;
}

HOOK(0x006B1150, CharacterModel_EvaluateMotionChannel, "auto");
