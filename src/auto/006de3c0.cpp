#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Signed division by 28, using the original compiler's reciprocal.
static int32_t GfxTextureLoader_CountRecords(uint32_t byteDifference)
{
    const int32_t difference = static_cast<int32_t>(byteDifference);
    const int64_t product =
        static_cast<int64_t>(difference) * static_cast<int32_t>(0x92492493);
    const int32_t highProduct = static_cast<int32_t>(product >> 32);
    const int32_t quotient = (highProduct + difference) >> 4;
    return quotient + static_cast<int32_t>(
        static_cast<uint32_t>(quotient) >> 31);
}

static char __fastcall S_006deed0_ReorderTextureRecords(
    S_006deed0* self, void* /*edx*/)
{
    const uint32_t groupsBegin = self->field_0004;
    const int32_t groupCount = groupsBegin
        ? (static_cast<int32_t>(self->field_0008 - groupsBegin) >> 4)
        : 0;
    const int32_t groupsToProcess = groupCount - 1;

    if (groupsToProcess <= 0)
        return static_cast<char>(groupCount);

    bool alreadyOrdered = true;
    auto* group = reinterpret_cast<uint8_t*>(groupsBegin);
    for (int32_t groupIndex = 0; groupIndex < groupsToProcess; ++groupIndex)
    {
        const uint32_t recordsBegin = CSF_AT(uint32_t, group, 0x04);
        const int32_t recordCount = recordsBegin
            ? GfxTextureLoader_CountRecords(
                  CSF_AT(uint32_t, group, 0x08) - recordsBegin)
            : 0;

        auto* record = reinterpret_cast<uint8_t*>(recordsBegin);
        for (int32_t index = 0; index < recordCount; ++index)
        {
            const int32_t position = CSF_AT(int32_t, record, 0x18);
            if (position != -1 && position > index)
                alreadyOrdered = false;
            record += 28;
        }
        group += 16;
    }

    if (alreadyOrdered)
        return 1;

    alignas(8) uint8_t savedRecord[28];
    uint32_t groupOffset = 0;
    for (int32_t groupIndex = 0; groupIndex < groupsToProcess; ++groupIndex)
    {
        group = reinterpret_cast<uint8_t*>(self->field_0004) + groupOffset;
        const uint32_t recordsBegin = CSF_AT(uint32_t, group, 0x04);
        if (recordsBegin)
        {
            const int32_t recordCount = GfxTextureLoader_CountRecords(
                CSF_AT(uint32_t, group, 0x08) - recordsBegin);

            uint32_t recordOffset = 0;
            for (int32_t index = 0; index < recordCount; ++index)
            {
                group = reinterpret_cast<uint8_t*>(self->field_0004)
                    + groupOffset;
                auto* record = reinterpret_cast<uint8_t*>(
                    CSF_AT(uint32_t, group, 0x04)) + recordOffset;
                const int32_t position = CSF_AT(int32_t, record, 0x18);

                if (position != -1 && index + 1 < recordCount)
                {
                    int32_t minimumPosition = position;
                    int32_t minimumIndex = -1;
                    auto* candidate = reinterpret_cast<uint8_t*>(
                        CSF_AT(uint32_t, group, 0x04)) + recordOffset + 28;

                    for (int32_t next = index + 1; next < recordCount; ++next)
                    {
                        const int32_t candidatePosition =
                            CSF_AT(int32_t, candidate, 0x18);
                        if (candidatePosition != -1
                            && candidatePosition < minimumPosition)
                        {
                            minimumIndex = next;
                            minimumPosition = candidatePosition;
                        }
                        candidate += 28;
                    }

                    if (minimumPosition < position)
                    {
                        const uint32_t positionOffset =
                            static_cast<uint32_t>(position) * 28;
                        const uint32_t minimumOffset =
                            static_cast<uint32_t>(minimumPosition) * 28;

                        auto* destination = reinterpret_cast<uint8_t*>(
                            CSF_AT(uint32_t, group, 0x14)) + positionOffset;
                        std::memcpy(savedRecord, destination, 28);
                        std::memcpy(
                            destination,
                            reinterpret_cast<uint8_t*>(
                                CSF_AT(uint32_t, group, 0x14)) + minimumOffset,
                            0x1C);

                        auto* currentGroup =
                            reinterpret_cast<uint8_t*>(self->field_0004)
                            + groupOffset;
                        std::memcpy(
                            reinterpret_cast<uint8_t*>(
                                CSF_AT(uint32_t, currentGroup, 0x14))
                                + minimumOffset,
                            savedRecord, 0x1C);

                        CSF_AT(int32_t,
                            reinterpret_cast<uint8_t*>(
                                CSF_AT(uint32_t, group, 0x04)) + recordOffset,
                            0x18) = minimumPosition;
                        CSF_AT(int32_t,
                            reinterpret_cast<uint8_t*>(
                                CSF_AT(uint32_t, group, 0x04))
                                + static_cast<uint32_t>(minimumIndex) * 28,
                            0x18) = position;
                    }
                }
                recordOffset += 28;
            }
        }
        groupOffset += 16;
    }

    return 0;
}

HOOK(0x006DE3C0, S_006deed0_ReorderTextureRecords, "auto");
