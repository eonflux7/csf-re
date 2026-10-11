#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __fastcall, S_007337a0_GetGroupWidth, 0x007337A0,
         (void* self, void* edx, int a2));
CSF_FUNC(double, __fastcall, S_007337a0_GetGroupExtraWidth, 0x00733830,
         (void* self, void* edx, int a2));
CSF_FUNC(double, __fastcall, S_007337a0_GetVerticalAlignmentSpace, 0x00733940,
         (void* self, void* edx));
CSF_FUNC(char, __fastcall, GrowPositionArray, 0x00734180,
         (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_AlignmentEpsilon, 0x008670C8);

// Retain the packet's vtable binding; this function does not install it.
static const auto kReportedVtable = CSF_VTBL(0x007337A0);

static int __fastcall S_007337a0_AppendAlignedPositions(
    S_007337a0* self, void* /*edx*/, uint32_t* source, int* destination)
{
    float horizontalOffset = 0.0f;
    float verticalOffset = 0.0f;
    int previousGroup = 0x7FFFFFFF;

    double verticalSpace =
        S_007337a0_GetVerticalAlignmentSpace(self, nullptr);
    if (verticalSpace > g_AlignmentEpsilon)
    {
        if (self->field_00bc == 1)
            verticalOffset = static_cast<float>(verticalSpace * g_HalfDurationScale);
        else if (self->field_00bc == 2)
            verticalOffset = static_cast<float>(verticalSpace);
    }

    uint32_t remaining = source[2];
    if (remaining == 0)
        return static_cast<int>(reinterpret_cast<uintptr_t>(source));

    uint32_t capacity = static_cast<uint32_t>(destination[1]);
    if (remaining > capacity)
        GrowPositionArray(destination, nullptr,
                          static_cast<int>(remaining - capacity));

    alignas(8) uint8_t extentBytes[0x1C];
    uint32_t sourceOffset = 0;

    do
    {
        auto* record = reinterpret_cast<uint8_t*>(source[0]) + sourceOffset;
        uint32_t value = CSF_AT(uint32_t, record, 0x04);
        float x = CSF_AT(float, record, 0x08);
        float y = CSF_AT(float, record, 0x0C);
        int group = CSF_AT(int, record, 0x2C);

        if (group != previousGroup)
        {
            previousGroup = group;
            float width = static_cast<float>(
                S_007337a0_GetGroupWidth(self, nullptr, group));
            double totalWidth =
                S_007337a0_GetGroupExtraWidth(self, nullptr, group) + width;
            width = static_cast<float>(totalWidth);

            int measuredWidth =
                static_cast<int32_t>(static_cast<int64_t>(totalWidth));
            if (measuredWidth <= static_cast<int32_t>(self->field_00d4))
                measuredWidth = static_cast<int32_t>(self->field_00d4);
            self->field_00d4 = static_cast<uint32_t>(measuredWidth);

            int* extent =
                ((int* (__fastcall*)(void*, void*, void*))
                     reinterpret_cast<void**>(self->field_0000)[0x5C / 4])(
                    self, nullptr, extentBytes);

            double horizontalSpace = static_cast<double>(*extent) - width;
            if (self->field_00b8 == 1)
                horizontalOffset =
                    static_cast<float>(horizontalSpace * g_HalfDurationScale);
            else if (self->field_00b8 == 2)
                horizontalOffset = static_cast<float>(horizontalSpace);
        }

        x += horizontalOffset;
        y += verticalOffset;

        if (static_cast<uint32_t>(destination[2]) >=
            static_cast<uint32_t>(destination[1]))
        {
            GrowPositionArray(destination, nullptr, destination[3]);
        }

        uint32_t index = static_cast<uint32_t>(destination[2]);
        auto* output = reinterpret_cast<uint8_t*>(
            static_cast<uint32_t>(destination[0]));
        destination[2] = static_cast<int>(index + 1);
        output += 12 * index;
        CSF_AT(uint32_t, output, 0x00) = value;
        CSF_AT(float, output, 0x04) = x;
        CSF_AT(float, output, 0x08) = y;

        sourceOffset += 0x30;
        --remaining;
    } while (remaining != 0);

    return 0;
}

HOOK(0x007345E0, S_007337a0_AppendAlignedPositions, "auto");
