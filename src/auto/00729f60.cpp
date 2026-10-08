#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ScaleBaseLayout, 0x00713E60,
         (C_00865a10* self, void* edx, int horizontalBits, int verticalBits));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static std::uint32_t TruncateScaledInteger(double value)
{
    return static_cast<std::uint32_t>(static_cast<std::int64_t>(value));
}

static std::uint32_t RoundScaledInteger(std::uint32_t value, float scale)
{
    double scaled = static_cast<double>(static_cast<std::int32_t>(value)) *
                    static_cast<double>(scale);

    // The x87 C0 test also takes the subtraction branch for unordered values.
    if (!(scaled >= static_cast<double>(g_fZero)))
        scaled -= static_cast<double>(g_HalfDurationScale);
    else
        scaled += static_cast<double>(g_HalfDurationScale);

    return TruncateScaledInteger(scaled);
}

static int __fastcall C_00865a10_ScaleLayout(
    C_00865a10* self, void* /*edx*/, int horizontalBits, int verticalBits)
{
    ScaleBaseLayout(self, nullptr, horizontalBits, verticalBits);

    float horizontalScale;
    float verticalScale;
    std::memcpy(&horizontalScale, &horizontalBits, sizeof(horizontalScale));
    std::memcpy(&verticalScale, &verticalBits, sizeof(verticalScale));

    std::uint32_t converted =
        RoundScaledInteger(self->field_0048, horizontalScale);
    std::uint32_t nextValue = self->field_004c;
    self->field_0048 = converted;
    self->field_004c = RoundScaledInteger(nextValue, verticalScale);

    std::uint32_t result = 0;
    if (self->field_0030 || self->field_0034 ||
        self->field_0038 || self->field_003c)
    {
        converted = RoundScaledInteger(self->field_0030, horizontalScale);
        nextValue = self->field_0038;
        self->field_0030 = converted;

        converted = RoundScaledInteger(nextValue, horizontalScale);
        nextValue = self->field_0034;
        self->field_0038 = converted;

        converted = RoundScaledInteger(nextValue, verticalScale);
        nextValue = self->field_003c;
        self->field_0034 = converted;

        result = RoundScaledInteger(nextValue, verticalScale);
        self->field_003c = result;
    }

    void* child = reinterpret_cast<void*>(self->field_0050);
    if (child)
    {
        const std::int32_t* source =
            reinterpret_cast<const std::int32_t* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, child, 0)[0x3c / 4])(child, nullptr);

        std::int32_t dimensions[2];
        dimensions[0] = source[0];
        dimensions[1] = source[1];

        converted = TruncateScaledInteger(
            static_cast<double>(dimensions[0]) * horizontalScale);
        const std::int32_t verticalDimension = dimensions[1];
        dimensions[0] = static_cast<std::int32_t>(converted);

        converted = TruncateScaledInteger(
            static_cast<double>(verticalDimension) * verticalScale);
        child = reinterpret_cast<void*>(self->field_0050);
        dimensions[1] = static_cast<std::int32_t>(converted);

        result = static_cast<std::uint32_t>(
            reinterpret_cast<int (__fastcall*)(void*, void*, std::int32_t*)>(
                CSF_AT(void**, child, 0)[0x38 / 4])(
                    child, nullptr, dimensions));
    }

    self->field_0058 = horizontalScale * self->field_0058;
    self->field_005c = verticalScale * self->field_005c;
    return static_cast<std::int32_t>(result);
}

HOOK(0x00729F60, C_00865a10_ScaleLayout, "auto");
