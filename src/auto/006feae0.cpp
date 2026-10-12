#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(const void*, __fastcall, Vec_GrowTwelveByteRecordArray, 0x004DE9B0, (void* self, void* , int extraRecords));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_RotationWrapStep, 0x0082E1E4);
CSF_GLOBAL(float, g_ObjectFadeInnerRadiusRatio, 0x00861C7C);
CSF_GLOBAL(float, g_BoxSampleFirstY, 0x008BBB4C);
CSF_GLOBAL(float, g_BoxSampleThirdCoordinate, 0x008BBB54);

static double __fastcall GfxClumpLoader_SampleBoxBelowPlane(
    void* self, void* /*edx*/, float* box, int* output)
{
    float origin[3];
    float firstStep[3];
    float secondStep[3];
    float thirdStep[3];

    for (int axis = 0; axis < 3; ++axis)
    {
        origin[axis] = box[axis]
            - box[3] * box[6 + axis]
            - box[4] * box[9 + axis]
            - box[5] * box[12 + axis];
        firstStep[axis] =
            (box[3] * g_ObjectFadeInnerRadiusRatio) * box[6 + axis];
        secondStep[axis] =
            (box[4] * g_ObjectFadeInnerRadiusRatio) * box[9 + axis];
        thirdStep[axis] =
            (box[5] * g_ObjectFadeInnerRadiusRatio) * box[12 + axis];
    }

    float thirdOffsets[4][3];
    for (int axis = 0; axis < 3; ++axis)
    {
        thirdOffsets[0][axis] = thirdStep[axis] * g_fZero;
        thirdOffsets[1][axis] = thirdStep[axis];
        thirdOffsets[2][axis] = thirdStep[axis] + thirdStep[axis];
        thirdOffsets[3][axis] = thirdStep[axis] * g_RotationWrapStep;
    }

    // The unsigned-to-floating conversion needs no explicit x87 bias.
    float* destination = &g_BoxSampleThirdCoordinate - 3;
    for (std::uint32_t first = 0; first < 4; ++first)
    {
        float row[3];
        for (int axis = 0; axis < 3; ++axis)
            row[axis] = firstStep[axis] * static_cast<float>(first) + origin[axis];

        for (std::uint32_t second = 0; second < 4; ++second)
        {
            float point[3];
            for (int axis = 0; axis < 3; ++axis)
                point[axis] = secondStep[axis] * static_cast<float>(second) + row[axis];

            for (int third = 0; third < 4; ++third)
            {
                destination[0] = thirdOffsets[third][0] + point[0];
                destination[1] = thirdOffsets[third][1] + point[1];
                destination[2] = thirdOffsets[third][2] + point[2];
                destination += 3;
            }
        }
    }

    CSF_AT(std::uint32_t, self, 0x3C) = 0;
    CSF_AT(std::uint32_t, self, 0x4C) = 0;

    std::uint32_t minimumIndex = 0;
    std::uint32_t maximumIndex = 0;
    float* sampleY = &g_BoxSampleFirstY;

    for (std::uint32_t byteOffset = 0, index = 0;
         byteOffset < 0x300; byteOffset += 12, ++index)
    {
        const float height = CSF_AT(float, sampleY, byteOffset);
        std::uint32_t* entry;

        if (height >= CSF_AT(float, self, 0x1C))
        {
            const std::uint32_t capacity = CSF_AT(std::uint32_t, self, 0x48);
            if (CSF_AT(std::uint32_t, self, 0x4C) >= capacity)
            {
                const std::uint32_t growth = CSF_AT(std::uint32_t, self, 0x50);
                void* replacement = Mem_Alloc(4 * (growth + capacity));
                const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x4C);
                if (count)
                    std::memcpy(replacement, CSF_AT(void*, self, 0x44), 4 * count);

                CSF_AT(std::uint32_t, self, 0x48) += growth;
                if (CSF_AT(void*, self, 0x44))
                {
                    Mem_Free(CSF_AT(void*, self, 0x44));
                    CSF_AT(void*, self, 0x44) = nullptr;
                }
                CSF_AT(void*, self, 0x44) = replacement;
            }

            const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x4C);
            entry = CSF_AT(std::uint32_t*, self, 0x44) + count;
            CSF_AT(std::uint32_t, self, 0x4C) = count + 1;
        }
        else
        {
            const std::uint32_t capacity = CSF_AT(std::uint32_t, self, 0x38);
            if (CSF_AT(std::uint32_t, self, 0x3C) >= capacity)
            {
                const std::uint32_t growth = CSF_AT(std::uint32_t, self, 0x40);
                void* replacement = Mem_Alloc(4 * (growth + capacity));
                const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x3C);
                if (count)
                    std::memcpy(replacement, CSF_AT(void*, self, 0x34), 4 * count);

                CSF_AT(std::uint32_t, self, 0x38) += growth;
                if (CSF_AT(void*, self, 0x34))
                {
                    Mem_Free(CSF_AT(void*, self, 0x34));
                    CSF_AT(void*, self, 0x34) = nullptr;
                }
                CSF_AT(void*, self, 0x34) = replacement;
            }

            const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x3C);
            entry = CSF_AT(std::uint32_t*, self, 0x34) + count;
            CSF_AT(std::uint32_t, self, 0x3C) = count + 1;
        }

        *entry = index;
        if (height < sampleY[3 * minimumIndex])
            minimumIndex = index;
        if (height > sampleY[3 * maximumIndex])
            maximumIndex = index;
    }

    if (!CSF_AT(std::uint32_t, self, 0x4C))
        return g_TransitionOne;

    const std::uint32_t belowCount = CSF_AT(std::uint32_t, self, 0x3C);
    if (!belowCount)
        return g_fZero;

    const std::uint32_t outputCapacity = static_cast<std::uint32_t>(output[1]);
    if (belowCount > outputCapacity)
        Vec_GrowTwelveByteRecordArray(
            output, nullptr, static_cast<int>(belowCount - outputCapacity));

    for (std::uint32_t index = 0;
         index < CSF_AT(std::uint32_t, self, 0x3C); ++index)
    {
        const std::uint32_t sampleIndex =
            CSF_AT(std::uint32_t*, self, 0x34)[index];
        const auto* source = reinterpret_cast<const std::uint32_t*>(
            reinterpret_cast<const std::uint8_t*>(sampleY) - 4 + 12 * sampleIndex);

        if (static_cast<std::uint32_t>(output[2]) >=
            static_cast<std::uint32_t>(output[1]))
        {
            Vec_GrowTwelveByteRecordArray(output, nullptr, output[3]);
        }

        const std::uint32_t count = static_cast<std::uint32_t>(output[2]);
        auto* record = reinterpret_cast<std::uint32_t*>(
            static_cast<std::uintptr_t>(static_cast<std::uint32_t>(output[0]))
            + 12 * count);
        output[2] = static_cast<int>(count + 1);
        record[0] = source[0];
        record[1] = source[1];
        record[2] = source[2];
    }

    return (CSF_AT(float, self, 0x1C) - sampleY[3 * minimumIndex])
        / (sampleY[3 * maximumIndex] - sampleY[3 * minimumIndex]);
}

HOOK(0x006FEAE0, GfxClumpLoader_SampleBoxBelowPlane, "auto");
