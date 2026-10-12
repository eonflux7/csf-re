#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void**, __fastcall, GfxCollision_AssignIntersectionArray, 0x006EC6D0,
         (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, GfxCollision_GrowIntersectionArray, 0x006EC910,
         (void* self, void* edx, int a2));

static int __fastcall GfxCollision_MergeSortedIntersections(
    uint8_t* self, void* /*edx*/, uint8_t* other)
{
    if (CSF_AT(uint8_t, self, 0x38) != 0)
    {
        std::memcpy(self + 0x1C, other + 0x1C, 0x1C);
        CSF_AT(uint8_t, self, 0x38) = 0;
    }

    alignas(8) uint8_t temporaryBytes[0x28];
    auto& temporaryData = CSF_AT(char*, temporaryBytes, 0x00);
    auto& temporaryCapacity = CSF_AT(uint32_t, temporaryBytes, 0x04);
    auto& temporaryCount = CSF_AT(uint32_t, temporaryBytes, 0x08);
    auto& growthAmount = CSF_AT(int, temporaryBytes, 0x0C);

    temporaryData = nullptr;
    temporaryCapacity = 0;
    temporaryCount = 0;
    growthAmount = 10;

    char* output = nullptr;
    uint32_t outputCount = 0;
    uint32_t outputCapacity = 0;
    uint32_t selfIndex = 0;
    uint32_t otherIndex = 0;
    uint32_t selfOffset = 0;
    uint32_t otherOffset = 0;

    if (CSF_AT(uint32_t, self, 0x0C) != 0)
    {
        do
        {
            if (otherIndex >= CSF_AT(uint32_t, other, 0x0C))
                break;

            const uint8_t* selfEntry =
                CSF_AT(uint8_t*, self, 0x04) + selfOffset;
            const uint8_t* otherEntry =
                CSF_AT(uint8_t*, other, 0x04) + otherOffset;

            const float selfDistance = CSF_AT(float, selfEntry, 0x00);
            const float otherDistance = CSF_AT(float, otherEntry, 0x00);

            if (selfDistance < otherDistance || selfDistance == otherDistance)
            {
                if (outputCount >= outputCapacity)
                {
                    GfxCollision_GrowIntersectionArray(
                        temporaryBytes, nullptr, growthAmount);
                    outputCount = temporaryCount;
                    output = temporaryData;
                }
                std::memcpy(output + 104 * outputCount++, selfEntry, 0x68);
                ++selfIndex;
                selfOffset += 104;
            }
            else
            {
                if (outputCount >= outputCapacity)
                {
                    GfxCollision_GrowIntersectionArray(
                        temporaryBytes, nullptr, growthAmount);
                    outputCount = temporaryCount;
                    output = temporaryData;
                }
                std::memcpy(output + 104 * outputCount++, otherEntry, 0x68);
                ++otherIndex;
                otherOffset += 104;
            }

            const uint32_t selfCount = CSF_AT(uint32_t, self, 0x0C);
            outputCapacity = temporaryCapacity;
            temporaryCount = outputCount;
            if (selfIndex >= selfCount)
                break;
        } while (true);
    }

    if (selfIndex < CSF_AT(uint32_t, self, 0x0C))
    {
        if (CSF_AT(uint8_t, other, 0x19) != 0)
        {
            CSF_AT(uint8_t, self, 0x19) = 1;
        }
        else
        {
            selfOffset = 104 * selfIndex;
            for (;;)
            {
                const uint8_t* entry =
                    CSF_AT(uint8_t*, self, 0x04) + selfOffset;
                if (outputCount >= outputCapacity)
                {
                    GfxCollision_GrowIntersectionArray(
                        temporaryBytes, nullptr, growthAmount);
                    outputCount = temporaryCount;
                    output = temporaryData;
                }

                const uint32_t previousIndex = selfIndex;
                std::memcpy(output + 104 * outputCount, entry, 0x68);
                const uint32_t selfCount = CSF_AT(uint32_t, self, 0x0C);
                temporaryCount = ++outputCount;
                selfIndex = previousIndex + 1;
                selfOffset += 104;

                if (selfIndex >= selfCount)
                    break;
                outputCapacity = temporaryCapacity;
            }
        }
    }
    else if (CSF_AT(uint8_t, self, 0x19) == 0)
    {
        if (otherIndex < CSF_AT(uint32_t, other, 0x0C))
        {
            otherOffset = 104 * otherIndex;
            for (;;)
            {
                const uint8_t* entry =
                    CSF_AT(uint8_t*, other, 0x04) + otherOffset;
                if (outputCount >= outputCapacity)
                {
                    GfxCollision_GrowIntersectionArray(
                        temporaryBytes, nullptr, growthAmount);
                    outputCount = temporaryCount;
                    output = temporaryData;
                }

                std::memcpy(output + 104 * outputCount++, entry, 0x68);
                const bool hasMore =
                    otherIndex + 1 < CSF_AT(uint32_t, other, 0x0C);
                temporaryCount = outputCount;
                ++otherIndex;
                otherOffset += 104;

                if (!hasMore)
                    break;
                outputCapacity = temporaryCapacity;
            }
        }
        CSF_AT(uint8_t, self, 0x19) = CSF_AT(uint8_t, other, 0x19);
    }

    GfxCollision_AssignIntersectionArray(
        self + 0x04, nullptr,
        static_cast<int>(reinterpret_cast<intptr_t>(temporaryBytes)));

    CSF_AT(uint8_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;

    if (output != nullptr)
        return Mem_Free(output);
    return static_cast<int>(reinterpret_cast<intptr_t>(self));
}

HOOK(0x006ED600, GfxCollision_MergeSortedIntersections, "auto");
