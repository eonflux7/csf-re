#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, S_00413570_GrowEventRecords, 0x00413570, (S_00413570* self, void* , int additionalCapacity));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static bool __fastcall EventQueue_TryConsumeRecord(
    void* self, void* /*edx*/, const void* incomingRecord)
{
    (void)CSF_VTBL(0x00413570);

    auto* records = reinterpret_cast<S_00413570*>(
        static_cast<std::uint8_t*>(self) + 0x2C);

    std::int32_t consumeLimit = CSF_AT(std::int32_t, self, 0x24);

    if (CSF_AT(std::int32_t, self, 0x28) < consumeLimit)
    {
        const std::uint32_t initialCount = records->field_0008;
        if (initialCount != 0)
        {
            std::int32_t index = static_cast<std::int32_t>(initialCount - 1);
            if (index >= 0)
            {
                std::uint32_t offset = 12 * static_cast<std::uint32_t>(index);
                do
                {
                    const std::uint32_t recordAddress = records->field_0000 + offset;
                    if (!CHandleTable_Resolve(
                            g_pCurrentScene, nullptr,
                            static_cast<int>(recordAddress)))
                    {
                        const std::uint32_t newCount = records->field_0008 - 1;
                        records->field_0008 = newCount;

                        if (static_cast<std::uint32_t>(index) != newCount)
                        {
                            auto* destination = reinterpret_cast<std::uint8_t*>(
                                records->field_0000 + offset);
                            _memcpy(
                                destination, destination + 12,
                                12 * (newCount - static_cast<std::uint32_t>(index)));
                        }
                    }

                    --index;
                    offset -= 12;
                } while (index >= 0);
            }
        }

        const std::int32_t remainingCount =
            static_cast<std::int32_t>(records->field_0008);

        if (remainingCount == 0)
        {
            ++CSF_AT(std::uint32_t, self, 0x28);
            return true;
        }

        if (consumeLimit > remainingCount)
            consumeLimit = remainingCount;

        if (consumeLimit > 0)
        {
            auto* candidate = reinterpret_cast<std::uint8_t*>(records->field_0000);
            const std::uint32_t incomingKey =
                CSF_AT(std::uint32_t, incomingRecord, 0x04);

            for (std::int32_t index = 0; index < consumeLimit; ++index)
            {
                if (CSF_AT(std::uint32_t, candidate, 0x04) == incomingKey)
                {
                    const std::uint32_t newCount = records->field_0008 - 1;
                    records->field_0008 = newCount;

                    if (static_cast<std::uint32_t>(index) != newCount)
                    {
                        auto* destination = reinterpret_cast<std::uint8_t*>(
                            records->field_0000 +
                            12 * static_cast<std::uint32_t>(index));
                        _memcpy(
                            destination, destination + 12,
                            12 * (newCount - static_cast<std::uint32_t>(index)));
                    }

                    ++CSF_AT(std::uint32_t, self, 0x28);
                    return true;
                }

                candidate += 12;
            }
        }
    }

    const std::uint32_t count = records->field_0008;
    bool found = false;

    if (count != 0)
    {
        auto* candidate = reinterpret_cast<std::uint8_t*>(records->field_0000);
        const std::uint32_t incomingKey =
            CSF_AT(std::uint32_t, incomingRecord, 0x04);

        for (std::uint32_t index = 0; index < count; ++index)
        {
            if (CSF_AT(std::uint32_t, candidate, 0x04) == incomingKey)
            {
                found = index != static_cast<std::uint32_t>(-1);
                break;
            }
            candidate += 12;
        }
    }

    if (!found)
    {
        if (records->field_0008 >= records->field_0004)
        {
            S_00413570_GrowEventRecords(
                records, nullptr, static_cast<int>(records->field_000c));
        }

        const std::uint32_t insertionIndex = records->field_0008;
        const std::uint32_t bufferAddress = records->field_0000;
        records->field_0008 = insertionIndex + 1;

        auto* destination = reinterpret_cast<std::uint8_t*>(
            bufferAddress + 12 * insertionIndex);
        CSF_AT(std::uint32_t, destination, 0x04) =
            CSF_AT(std::uint32_t, incomingRecord, 0x04);
        CSF_AT(std::uint32_t, destination, 0x08) =
            CSF_AT(std::uint32_t, incomingRecord, 0x08);
    }

    return false;
}

HOOK(0x00449230, EventQueue_TryConsumeRecord, "auto");
