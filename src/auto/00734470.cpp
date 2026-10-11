#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __fastcall, S_007337a0_GetGroupLeadingExtent, 0x007337A0,
         (void* self, void* edx, int a2));
CSF_FUNC(double, __fastcall, S_007337a0_GetGroupTrailingExtent, 0x00733830,
         (void* self, void* edx, int a2));
CSF_FUNC(double, __fastcall, S_007337a0_GetBlockExtent, 0x00733940,
         (void* self, void* edx));
CSF_FUNC(char, __fastcall, GrowAlignedElementBuffer, 0x007340D0,
         (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_AlignmentEpsilon, 0x008670C8);

// Retain the vtable binding explicitly listed in the packet.
[[maybe_unused]] static const auto g_GroupExtentVtable = CSF_VTBL(0x007337A0);

static char __fastcall S_007337a0_AppendAlignedElements(
    S_007337a0* self, void* /*edx*/, uint32_t* source, uint32_t* destination)
{
    float horizontalOffset = 0.0f;
    float verticalOffset = 0.0f;
    int previousGroup = 0x7FFFFFFF;

    const double blockExtent =
        S_007337a0_GetBlockExtent(self, nullptr);
    if (blockExtent > g_AlignmentEpsilon)
    {
        if (self->field_00bc == 1)
            verticalOffset = static_cast<float>(blockExtent * g_HalfDurationScale);
        else if (self->field_00bc == 2)
            verticalOffset = static_cast<float>(blockExtent);
    }

    uint32_t remaining = source[2];
    if (remaining == 0)
        return static_cast<char>(reinterpret_cast<uintptr_t>(source));

    if (remaining > destination[1])
    {
        GrowAlignedElementBuffer(
            destination, nullptr,
            static_cast<int>(remaining - destination[1]));
    }

    uint32_t sourceOffset = 0;
    alignas(8) uint8_t dimensionsBytes[0x20];

    do
    {
        const uint8_t* sourceElement =
            reinterpret_cast<const uint8_t*>(source[0]) + sourceOffset;
        const int group = CSF_AT(int, sourceElement, 0x2C);
        float element[7];
        std::memcpy(element, sourceElement + 0x10, sizeof(element));

        if (group != previousGroup)
        {
            previousGroup = group;
            float groupExtent = static_cast<float>(
                S_007337a0_GetGroupLeadingExtent(self, nullptr, group));
            const double combinedExtent =
                S_007337a0_GetGroupTrailingExtent(self, nullptr, group)
                + groupExtent;
            groupExtent = static_cast<float>(combinedExtent);

            int extent = static_cast<int>(
                static_cast<int64_t>(combinedExtent));
            if (extent <= static_cast<int>(self->field_00d4))
                extent = static_cast<int>(self->field_00d4);
            self->field_00d4 = static_cast<uint32_t>(extent);

            using GetDimensions = int* (__fastcall*)(
                S_007337a0*, void*, void*);
            int* dimensions =
                reinterpret_cast<GetDimensions>(
                    reinterpret_cast<void**>(self->field_0000)[0x5C / 4])(
                        self, nullptr, dimensionsBytes);

            const double unusedExtent =
                static_cast<double>(*dimensions) - groupExtent;
            if (self->field_00b8 == 1)
                horizontalOffset =
                    static_cast<float>(unusedExtent * g_HalfDurationScale);
            else if (self->field_00b8 == 2)
                horizontalOffset = static_cast<float>(unusedExtent);
        }

        const bool hasCapacity = destination[2] < destination[1];
        element[1] += horizontalOffset;
        element[2] += verticalOffset;

        if (!hasCapacity)
        {
            GrowAlignedElementBuffer(
                destination, nullptr, static_cast<int>(destination[3]));
        }

        const uint32_t destinationIndex = destination[2];
        uint8_t* output =
            reinterpret_cast<uint8_t*>(destination[0])
            + destinationIndex * 0x1C;
        destination[2] = destinationIndex + 1;
        std::memcpy(output, element, 0x1C);

        sourceOffset += 0x30;
        --remaining;
    } while (remaining != 0);

    return 0;
}

HOOK(0x00734470, S_007337a0_AppendAlignedElements, "auto");
