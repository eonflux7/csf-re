#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00863f60_ScaleCoordinates, 0x00713E60,
         (C_008657e0* self, void* edx, int horizontalBits, int verticalBits));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static std::uint32_t ScaleRoundedCoordinate(std::int32_t coordinate, float scale)
{
    double value = static_cast<double>(coordinate) * static_cast<double>(scale);

    // FCOM sets C0 for both less-than and unordered comparisons.
    if (!(value >= static_cast<double>(g_fZero)))
        value -= static_cast<double>(g_HalfDurationScale);
    else
        value += static_cast<double>(g_HalfDurationScale);

    return static_cast<std::uint32_t>(static_cast<std::int64_t>(value));
}

static int __fastcall C_008657e0_ScaleCoordinates(
    C_008657e0* self, void* /*edx*/, int horizontalBits, int verticalBits)
{
    C_00863f60_ScaleCoordinates(self, nullptr, horizontalBits, verticalBits);

    float horizontalScale;
    float verticalScale;
    std::memcpy(&horizontalScale, &horizontalBits, sizeof(horizontalScale));
    std::memcpy(&verticalScale, &verticalBits, sizeof(verticalScale));

    std::uint32_t rounded = ScaleRoundedCoordinate(
        static_cast<std::int32_t>(self->field_0034), horizontalScale);
    std::int32_t nextCoordinate = static_cast<std::int32_t>(self->field_0038);
    self->field_0034 = rounded;

    self->field_0038 = ScaleRoundedCoordinate(nextCoordinate, verticalScale);

    if (self->field_003c == 0 &&
        self->field_0040 == 0 &&
        self->field_0044 == 0 &&
        self->field_0048 == 0)
    {
        return 0;
    }

    rounded = ScaleRoundedCoordinate(
        static_cast<std::int32_t>(self->field_003c), horizontalScale);
    nextCoordinate = static_cast<std::int32_t>(self->field_0044);
    self->field_003c = rounded;

    rounded = ScaleRoundedCoordinate(nextCoordinate, horizontalScale);
    nextCoordinate = static_cast<std::int32_t>(self->field_0040);
    self->field_0044 = rounded;

    rounded = ScaleRoundedCoordinate(nextCoordinate, verticalScale);
    nextCoordinate = static_cast<std::int32_t>(self->field_0048);
    self->field_0040 = rounded;

    rounded = ScaleRoundedCoordinate(nextCoordinate, verticalScale);
    self->field_0048 = rounded;
    return static_cast<std::int32_t>(rounded);
}

HOOK(0x00729000, C_008657e0_ScaleCoordinates, "auto");
