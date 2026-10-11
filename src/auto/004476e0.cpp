#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, S_00413570_GrowEventRecords, 0x00413570, (S_00413570* self, void* , int additionalCapacity));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

[[maybe_unused]] static const auto g_EventRecordsVtable = CSF_VTBL(0x00413570);

static char __fastcall EventRecords_TryAcceptOrQueue(
    void* self, void* /*edx*/, const void* event)
{
    auto* records = reinterpret_cast<S_00413570*>(
        static_cast<std::uint8_t*>(self) + 0xC8);

    std::int32_t limit = CSF_AT(std::int32_t, self, 0xC0);
    if (CSF_AT(std::int32_t, self, 0xC4) < limit)
    {
        const std::int32_t count =
            static_cast<std::int32_t>(records->field_0008);

        if (count == 0)
        {
            ++CSF_AT(std::uint32_t, self, 0xC4);
            return 1;
        }

        if (limit > count)
            limit = count;

        if (limit > 0)
        {
            auto* record = reinterpret_cast<std::uint8_t*>(
                records->field_0000);
            const std::uint32_t key = CSF_AT(std::uint32_t, event, 0x4);

            for (std::int32_t index = 0; index < limit; ++index, record += 12)
            {
                if (CSF_AT(std::uint32_t, record, 0x4) != key)
                    continue;

                const std::uint32_t remaining = records->field_0008 - 1;
                records->field_0008 = remaining;

                if (static_cast<std::uint32_t>(index) != remaining)
                {
                    auto* destination = reinterpret_cast<std::uint8_t*>(
                        records->field_0000)
                        + 12 * static_cast<std::uint32_t>(index);

                    _memcpy(destination, destination + 12,
                        12 * (remaining - static_cast<std::uint32_t>(index)));
                }

                ++CSF_AT(std::uint32_t, self, 0xC4);
                return 1;
            }
        }
    }

    const std::uint32_t count = records->field_0008;
    if (count != 0)
    {
        auto* record = reinterpret_cast<std::uint8_t*>(
            records->field_0000);
        const std::uint32_t key = CSF_AT(std::uint32_t, event, 0x4);

        for (std::uint32_t index = 0; index < count; ++index, record += 12)
        {
            if (CSF_AT(std::uint32_t, record, 0x4) == key)
            {
                if (index != static_cast<std::uint32_t>(-1))
                    return 0;
                break;
            }
        }
    }

    if (records->field_0008 >= records->field_0004)
    {
        S_00413570_GrowEventRecords(
            records, nullptr, static_cast<int>(records->field_000c));
    }

    const std::uint32_t index = records->field_0008;
    auto* storage = reinterpret_cast<std::uint8_t*>(records->field_0000);
    records->field_0008 = index + 1;

    auto* record = storage + 12 * index;
    CSF_AT(std::uint32_t, record, 0x4) =
        CSF_AT(std::uint32_t, event, 0x4);
    CSF_AT(std::uint32_t, record, 0x8) =
        CSF_AT(std::uint32_t, event, 0x8);
    return 0;
}

HOOK(0x004476E0, EventRecords_TryAcceptOrQueue, "auto");
