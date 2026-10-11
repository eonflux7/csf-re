#include <cstdint>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetIndexedTableValue_6AF8A0, 0x006AF8A0, (void* self, void* , int index));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static double __fastcall S_007337a0_UpdateUnoccupiedDuration(
    S_007337a0* self, void* /*edx*/)
{
    float firstLeadingGap = FLT_MAX;
    float firstTrailingGap = FLT_MAX;

    const uint32_t firstCount = self->field_0060;
    if (firstCount)
    {
        auto* entries = reinterpret_cast<uint8_t*>(self->field_0058);
        void* source = reinterpret_cast<void*>(self->field_0014);

        firstLeadingGap = static_cast<float>(
            CSF_AT(float, entries, 0x18) -
            static_cast<double>(static_cast<int32_t>(self->field_009c)));

        const float trailingGap = static_cast<float>(
            static_cast<double>(static_cast<int32_t>(self->field_00a4)) -
            CSF_AT(float, entries + 48 * firstCount, -0x18));

        const double duration =
            reinterpret_cast<double(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, source, 0)[0x1c / 4])(source, nullptr, 1);
        firstTrailingGap = static_cast<float>(trailingGap - duration);
    }

    float secondLeadingGap = FLT_MAX;
    float secondTrailingGap = FLT_MAX;

    const uint32_t secondCount = self->field_0070;
    if (secondCount)
    {
        auto* entries = reinterpret_cast<uint8_t*>(self->field_0068);
        const int index = CSF_AT(int, entries + 48 * secondCount, -0x2c);

        secondLeadingGap = static_cast<float>(
            CSF_AT(float, entries, 0x0c) -
            static_cast<double>(static_cast<int32_t>(self->field_009c)));

        const int indexedValue = GetIndexedTableValue_6AF8A0(
            reinterpret_cast<void*>(self->field_0008), nullptr, index);

        double duration;
        if (indexedValue)
        {
            void* object = reinterpret_cast<void*>(indexedValue);
            void* durationData =
                reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, object, 0)[0x40 / 4])(object, nullptr);
            duration = CSF_AT(float, durationData, 0x04);
        }
        else
        {
            duration = g_fZero;
        }

        secondTrailingGap = static_cast<float>(
            static_cast<double>(static_cast<int32_t>(self->field_00a4)) -
            CSF_AT(float,
                reinterpret_cast<uint8_t*>(self->field_0068) +
                    48 * self->field_0070,
                -0x24) -
            duration);
    }

    const float leadingGap = firstLeadingGap < secondLeadingGap
        ? firstLeadingGap : secondLeadingGap;
    const float trailingGap = firstTrailingGap < secondTrailingGap
        ? firstTrailingGap : secondTrailingGap;
    const double result = static_cast<double>(leadingGap) + trailingGap;

    const int32_t interval =
        static_cast<int32_t>(self->field_00a4 - self->field_009c);
    const double remaining = static_cast<double>(interval) - result;
    const double rounded = remaining >= g_fZero
        ? remaining + g_HalfDurationScale
        : remaining - g_HalfDurationScale;

    self->field_00d0 = static_cast<uint32_t>(static_cast<int64_t>(rounded));
    return result;
}

HOOK(0x00733940, S_007337a0_UpdateUnoccupiedDuration, "auto");
