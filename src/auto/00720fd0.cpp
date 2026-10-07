#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_fPi, 0x00864BBC);

static double EvaluateX87Sine(double angle)
{
    // FSIN leaves finite arguments outside its supported range unchanged.
    const double rangeLimit = static_cast<double>(std::uint64_t{1} << 63);
    if (std::isfinite(angle) && std::fabs(angle) >= rangeLimit)
        return angle;

    return std::sin(angle);
}

static void __fastcall UpdateSineInterpolatedValue(
    void* self, void* /*edx*/, float deltaTime)
{
    if (CSF_AT(std::uint32_t, self, 0x134) != 0 &&
        CSF_AT(float, self, 0x118) != g_fZero &&
        CSF_AT(std::uint8_t, self, 0x154) != 0)
    {
        const double duration = CSF_AT(float, self, 0x118);
        double elapsed = static_cast<double>(deltaTime) +
                         CSF_AT(float, self, 0x144);

        const bool resetElapsed = elapsed >= duration;
        CSF_AT(float, self, 0x144) = static_cast<float>(elapsed);

        // Unordered comparisons retain elapsed, matching FCOM's C0 flag.
        if (resetElapsed)
            elapsed = g_fZero;

        CSF_AT(float, self, 0x144) = static_cast<float>(elapsed);

        const double angle = (elapsed / duration) * g_fPi;
        const double sine = EvaluateX87Sine(angle);
        const double amplitude =
            static_cast<double>(CSF_AT(float, self, 0x150)) -
            CSF_AT(float, self, 0x14C);

        CSF_AT(float, self, 0x148) = static_cast<float>(
            sine * amplitude + CSF_AT(float, self, 0x14C));
    }
    else
    {
        CSF_AT(std::uint32_t, self, 0x148) =
            CSF_AT(std::uint32_t, self, 0x150);
    }
}

HOOK(0x00720FD0, UpdateSineInterpolatedValue, "auto");
