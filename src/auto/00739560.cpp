#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static std::uint64_t __fastcall C_00867444_GetRoundedScaledValue(
    C_00867444* self, void* /*edx*/, int value)
{
    const double scaledValue =
        reinterpret_cast<double(__fastcall*)(C_00867444*, void*, int)>(
            self->vtbl[7])(self, nullptr, value);

    const double roundedValue = scaledValue >= g_fZero
        ? scaledValue + g_HalfDurationScale
        : scaledValue - g_HalfDurationScale;

    return static_cast<std::uint64_t>(
        static_cast<std::int64_t>(roundedValue));
}

HOOK(0x00739560, C_00867444_GetRoundedScaledValue, "auto");
