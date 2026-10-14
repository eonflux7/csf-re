#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __cdecl, NormalizeDegreesAngle, 0x00439B20, (float angleDegrees));
CSF_FUNC(int, __fastcall, S_00439ba0_FlushAndProcessRecords, 0x00439C80, (S_00439ba0* self, void*));
CSF_FUNC(int, __fastcall, S_00439f80_GrowCapacity, 0x00439F80, (S_00439f80* self, void* , int additionalCapacity));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static char __fastcall S_00439ba0_BuildAndFlushSceneMenuRecords(
    S_00439ba0* self, void* /*edx*/)
{
    const char pending = static_cast<char>(self->field_004d);
    if (!pending)
        return pending;

    struct SegmentRecord
    {
        float startX;
        float startY;
        float endX;
        float endY;
        uint32_t sourceOffset;
        uint32_t sourceRow;
        uint32_t sourceLength;
        float parameter;
    };
    static_assert(sizeof(SegmentRecord) == 0x20);

    const float angleDegrees = self->field_0048;
    self->field_003c = 0;
    double angle = NormalizeDegreesAngle(angleDegrees);
    angle -= static_cast<double>(self->field_0010) * g_HalfDurationScale;
    if (angle < g_fZero)
    {
        angle *= g_MissingMetricValue;
        angle = NormalizeDegreesAngle(static_cast<float>(angle));
    }

    const double inverseAngularStep =
        static_cast<double>(g_TransitionOne) / CSF_AT(float, self, 0x14);
    const float interpolationScale = static_cast<float>(inverseAngularStep);
    const int32_t segmentIndex = static_cast<int32_t>(
        static_cast<int64_t>(inverseAngularStep * angle));

    const uint32_t sourceRow = self->field_0004;
    const uint32_t sourceStart = self->field_0000;
    const double angularRemainder =
        angle - static_cast<double>(segmentIndex) * CSF_AT(float, self, 0x14);

    SegmentRecord record;
    record.startX = CSF_AT(float, self, 0x18);
    record.startY = CSF_AT(float, self, 0x1C);
    record.endX = CSF_AT(float, self, 0x20);
    record.endY = CSF_AT(float, self, 0x24);

    uint32_t leadingLength = 0;
    if (angularRemainder > g_fZero)
    {
        const double span =
            static_cast<double>(CSF_AT(float, self, 0x20)) -
            CSF_AT(float, self, 0x18);
        record.endX = CSF_AT(float, self, 0x20);
        record.sourceRow = self->field_0004;
        record.parameter = CSF_AT(float, self, 0x0C);
        record.sourceOffset = sourceStart;
        record.startX = static_cast<float>(
            span * angularRemainder * interpolationScale +
            CSF_AT(float, self, 0x18));

        leadingLength = static_cast<uint32_t>(static_cast<int64_t>(
            static_cast<double>(CSF_AT(int32_t, self, 0x28)) / span *
            (static_cast<double>(record.endX) - record.startX)));
        record.sourceLength = leadingLength;

        if (self->field_003c >= CSF_AT(uint32_t, self, 0x38))
        {
            S_00439f80_GrowCapacity(
                reinterpret_cast<S_00439f80*>(
                    reinterpret_cast<uint8_t*>(self) + 0x34),
                nullptr, CSF_AT(int, self, 0x40));
        }

        const uint32_t index = self->field_003c;
        uint8_t* const records = CSF_AT(uint8_t*, self, 0x34);
        self->field_003c = index + 1;
        std::memcpy(records + index * 0x20, &record, 0x20);
    }

    const int32_t remainingLength =
        static_cast<int32_t>(self->field_0008 - leadingLength);
    const uint32_t fullLength = CSF_AT(uint32_t, self, 0x28);
    uint32_t currentOffset = sourceStart + leadingLength;

    const uint32_t fullSegments = static_cast<uint32_t>(
        static_cast<int64_t>(
            static_cast<double>(remainingLength) /
            static_cast<int32_t>(fullLength)));
    const uint32_t fullEnd = currentOffset + fullLength * fullSegments;

    record.startX = CSF_AT(float, self, 0x18);
    record.startY = CSF_AT(float, self, 0x1C);
    record.endX = CSF_AT(float, self, 0x20);
    record.endY = CSF_AT(float, self, 0x24);
    record.sourceRow = sourceRow;
    record.sourceOffset = currentOffset;
    record.sourceLength = fullLength;
    record.parameter = CSF_AT(float, self, 0x2C);

    if (static_cast<int32_t>(currentOffset) < static_cast<int32_t>(fullEnd))
    {
        do
        {
            record.sourceOffset = currentOffset;
            if (self->field_003c >= CSF_AT(uint32_t, self, 0x38))
            {
                S_00439f80_GrowCapacity(
                    reinterpret_cast<S_00439f80*>(
                        reinterpret_cast<uint8_t*>(self) + 0x34),
                    nullptr, CSF_AT(int, self, 0x40));
            }

            const uint32_t index = self->field_003c;
            uint8_t* const records = CSF_AT(uint8_t*, self, 0x34);
            self->field_003c = index + 1;
            std::memcpy(records + index * 0x20, &record, 0x20);

            currentOffset += CSF_AT(uint32_t, self, 0x28);
        }
        while (static_cast<int32_t>(currentOffset) <
               static_cast<int32_t>(fullEnd));
    }

    const int32_t trailingLength = static_cast<int32_t>(
        self->field_0000 + self->field_0008 - fullEnd);
    if (trailingLength > 0)
    {
        const double span =
            static_cast<double>(CSF_AT(float, self, 0x20)) -
            CSF_AT(float, self, 0x18);
        record.startX = CSF_AT(float, self, 0x18);
        const double segmentLength = CSF_AT(int32_t, self, 0x28);
        record.sourceOffset = currentOffset;
        record.sourceRow = sourceRow;
        record.parameter = CSF_AT(float, self, 0x0C);
        record.endX = static_cast<float>(
            static_cast<double>(trailingLength) * span / segmentLength +
            CSF_AT(float, self, 0x18));
        record.sourceLength = static_cast<uint32_t>(static_cast<int64_t>(
            (static_cast<double>(record.endX) - record.startX) /
            span * segmentLength));

        if (self->field_003c >= CSF_AT(uint32_t, self, 0x38))
        {
            S_00439f80_GrowCapacity(
                reinterpret_cast<S_00439f80*>(
                    reinterpret_cast<uint8_t*>(self) + 0x34),
                nullptr, CSF_AT(int, self, 0x40));
        }

        const uint32_t index = self->field_003c;
        uint8_t* const records = CSF_AT(uint8_t*, self, 0x34);
        self->field_003c = index + 1;
        std::memcpy(records + index * 0x20, &record, 0x20);
    }

    return static_cast<char>(
        S_00439ba0_FlushAndProcessRecords(self, nullptr));
}

HOOK(0x0043A180, S_00439ba0_BuildAndFlushSceneMenuRecords, "auto");
