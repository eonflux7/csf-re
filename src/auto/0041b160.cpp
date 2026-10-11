#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, Vehicle_GrowRecordArray, 0x004189D0,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Vehicle_SerializeRecord, 0x0041AAC0,
         (void* self, void* edx, int a2));

static int __stdcall Vehicle_SerializeRecordArray(void* records, int contextAddress)
{
    void* context = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(static_cast<std::uint32_t>(contextAddress)));

    const std::uint8_t mode = CSF_AT(std::uint8_t, context, 0x14);
    const std::uint8_t existingRecords = CSF_AT(std::uint8_t, context, 0x15);
    std::uint32_t count = 0;

    if (mode)
        count = CSF_AT(std::uint32_t, records, 0x08);

    const bool failed =
        CSF_AT(std::uint8_t, context, 0x00) == 0 &&
        !((bool(__fastcall*)(void*, void*, std::uint32_t*, int))
              CSF_AT(void**, CSF_AT(void*, context, 0x04), 0x00)[120 / 4])(
            CSF_AT(void*, context, 0x04), nullptr, &count, contextAddress);

    CSF_AT(std::uint8_t, context, 0x00) = failed;

    if (!failed)
    {
        if (mode || existingRecords)
        {
            std::uint32_t index = 0;
            std::uint32_t offset = 0;
            while (index < count)
            {
                void* record = reinterpret_cast<void*>(
                    static_cast<std::uintptr_t>(
                        CSF_AT(std::uint32_t, records, 0x00) + offset));
                const bool stopped = CSF_AT(std::uint8_t, context, 0x00) != 0;
                CSF_AT(std::uint32_t, context, 0x1C) = 0;
                if (!stopped)
                {
                    CSF_AT(std::uint8_t, context, 0x15) = existingRecords;
                    CSF_AT(std::uint8_t, context, 0x16) = 0;
                    Vehicle_SerializeRecord(record, nullptr, contextAddress);
                }
                ++index;
                offset += 24;
            }
        }
        else
        {
            const std::uint32_t capacity = CSF_AT(std::uint32_t, records, 0x04);
            std::uint32_t iterationCount = count;
            CSF_AT(std::uint32_t, records, 0x08) = 0;
            if (count > capacity)
            {
                Vehicle_GrowRecordArray(records, nullptr,
                                        static_cast<int>(count - capacity));
                iterationCount = count;
            }

            std::uint32_t index = 0;
            std::uint32_t offset = 0;
            if (iterationCount)
            {
                do
                {
                    if (CSF_AT(std::uint32_t, records, 0x08) >=
                        CSF_AT(std::uint32_t, records, 0x04))
                    {
                        Vehicle_GrowRecordArray(
                            records, nullptr, CSF_AT(int, records, 0x0C));
                    }

                    const std::uint32_t used = CSF_AT(std::uint32_t, records, 0x08);
                    void* newRecord = reinterpret_cast<void*>(
                        static_cast<std::uintptr_t>(
                            CSF_AT(std::uint32_t, records, 0x00) + 24 * used));
                    CSF_AT(std::uint32_t, records, 0x08) = used + 1;
                    CSF_AT(std::uint32_t, newRecord, 0x00) = 0;
                    CSF_AT(std::uint32_t, newRecord, 0x04) = 0;
                    CSF_AT(std::uint32_t, newRecord, 0x08) = 0;
                    CSF_AT(std::uint32_t, newRecord, 0x0C) = 0;
                    CSF_AT(std::uint32_t, newRecord, 0x10) = 0;
                    CSF_AT(std::uint32_t, newRecord, 0x14) = 0;

                    void* record = reinterpret_cast<void*>(
                        static_cast<std::uintptr_t>(
                            CSF_AT(std::uint32_t, records, 0x00) + offset));
                    const bool stopped = CSF_AT(std::uint8_t, context, 0x00) != 0;
                    CSF_AT(std::uint32_t, context, 0x1C) = 0;
                    if (!stopped)
                    {
                        CSF_AT(std::uint8_t, context, 0x15) = 0;
                        CSF_AT(std::uint8_t, context, 0x16) = 0;
                        Vehicle_SerializeRecord(record, nullptr, contextAddress);
                    }

                    ++index;
                    offset += 24;
                }
                while (index < count);
            }
        }

        if (CSF_AT(std::uint8_t, context, 0x00) == 0 &&
            ((bool(__fastcall*)(void*, void*, int))
                 CSF_AT(void**, CSF_AT(void*, context, 0x04), 0x00)[124 / 4])(
                CSF_AT(void*, context, 0x04), nullptr, contextAddress))
        {
            CSF_AT(std::uint8_t, context, 0x00) = 0;
            return 0;
        }
    }

    CSF_AT(std::uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x0041B160, Vehicle_SerializeRecordArray, "auto");
