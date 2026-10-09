#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, RecordArray_Grow, 0x0044F990, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Record_Serialize, 0x00453770, (void* self, void* edx, int a2));

static int __stdcall Game_SerializeRecordArray(std::uint32_t* records, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    const std::uint8_t savedMode = CSF_AT(std::uint8_t, context, 0x15);
    const bool useExistingCount = CSF_AT(std::uint8_t, context, 0x14) != 0;
    const std::uint8_t initialError = CSF_AT(std::uint8_t, context, 0x00);

    // The original leaves this unset when it skips the count callback.
    std::uint32_t count = 0;
    if (useExistingCount)
        count = CSF_AT(std::uint32_t, records, 0x08);

    bool countFailed = false;
    if (!initialError)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        countFailed =
            !reinterpret_cast<bool (__fastcall*)(void*, void*, std::uint32_t*, int)>(
                CSF_AT(void**, serializer, 0x00)[0x78 / 4])(
                    serializer, nullptr, &count, contextAddress);
    }

    CSF_AT(std::uint8_t, context, 0x00) = countFailed;
    if (!countFailed)
    {
        if (useExistingCount || savedMode)
        {
            std::uint32_t index = 0;
            std::uint32_t offset = 0;
            while (index < count)
            {
                void* record = CSF_AT(std::uint8_t*, records, 0x00) + offset;
                const bool canSerialize = CSF_AT(std::uint8_t, context, 0x00) == 0;
                CSF_AT(std::uint32_t, context, 0x1C) = 0;
                if (canSerialize)
                {
                    CSF_AT(std::uint8_t, context, 0x15) = savedMode;
                    CSF_AT(std::uint8_t, context, 0x16) = 0;
                    Record_Serialize(record, nullptr, contextAddress);
                }
                ++index;
                offset += 48;
            }
        }
        else
        {
            const std::uint32_t capacity = CSF_AT(std::uint32_t, records, 0x04);
            CSF_AT(std::uint32_t, records, 0x08) = 0;
            if (count > capacity)
                RecordArray_Grow(records, nullptr, static_cast<int>(count - capacity));

            if (count)
            {
                const std::uint32_t emptyRecord[12] = {};
                std::uint32_t index = 0;
                std::uint32_t offset = 0;
                do
                {
                    if (CSF_AT(std::uint32_t, records, 0x08) >=
                        CSF_AT(std::uint32_t, records, 0x04))
                    {
                        RecordArray_Grow(
                            records, nullptr,
                            static_cast<int>(CSF_AT(std::uint32_t, records, 0x0C)));
                    }

                    const std::uint32_t length = CSF_AT(std::uint32_t, records, 0x08);
                    void* appendedRecord =
                        CSF_AT(std::uint8_t*, records, 0x00) + 48 * length;
                    CSF_AT(std::uint32_t, records, 0x08) = length + 1;
                    std::memcpy(appendedRecord, emptyRecord, 0x30);

                    void* record = CSF_AT(std::uint8_t*, records, 0x00) + offset;
                    const bool canSerialize = CSF_AT(std::uint8_t, context, 0x00) == 0;
                    CSF_AT(std::uint32_t, context, 0x1C) = 0;
                    if (canSerialize)
                    {
                        CSF_AT(std::uint8_t, context, 0x15) = 0;
                        CSF_AT(std::uint8_t, context, 0x16) = 0;
                        Record_Serialize(record, nullptr, contextAddress);
                    }

                    ++index;
                    offset += 48;
                } while (index < count);
            }
        }

        if (!CSF_AT(std::uint8_t, context, 0x00))
        {
            void* serializer = CSF_AT(void*, context, 0x04);
            if (reinterpret_cast<bool (__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, serializer, 0x00)[0x7C / 4])(
                        serializer, nullptr, contextAddress))
            {
                CSF_AT(std::uint8_t, context, 0x00) = 0;
                return 0;
            }
        }
    }

    CSF_AT(std::uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x00454CF0, Game_SerializeRecordArray, "auto");
