#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowRecordArray, 0x00443A70,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ProcessRecord, 0x00444050,
         (void* self, void* edx, int a2));

static int __stdcall ProcessRecordArray(std::uint32_t* array, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    const std::uint8_t writing = CSF_AT(std::uint8_t, context, 0x14);
    const std::uint8_t existingRecords = CSF_AT(std::uint8_t, context, 0x15);

    alignas(8) std::uint8_t countBytes[0x4];
    auto& count = *reinterpret_cast<std::uint32_t*>(countBytes);
    count = 0;
    if (writing)
        count = CSF_AT(std::uint32_t, array, 0x8);

    bool failed = false;
    if (CSF_AT(std::uint8_t, context, 0x0) == 0)
    {
        void* handler = CSF_AT(void*, context, 0x4);
        failed = !reinterpret_cast<bool(__fastcall*)(
            void*, void*, std::uint32_t*, int)>(
                CSF_AT(void**, handler, 0x0)[0x78 / 4])(
                    handler, nullptr, &count, contextAddress);
    }
    CSF_AT(std::uint8_t, context, 0x0) = failed;

    if (!failed)
    {
        if (writing || existingRecords)
        {
            std::uint32_t index = 0;
            std::uint32_t offset = 0;
            while (index < count)
            {
                void* record = reinterpret_cast<void*>(
                    CSF_AT(std::uint32_t, array, 0x0) + offset);
                const bool canProcess =
                    CSF_AT(std::uint8_t, context, 0x0) == 0;
                CSF_AT(std::uint32_t, context, 0x1C) = 0;
                if (canProcess)
                {
                    CSF_AT(std::uint8_t, context, 0x15) = existingRecords;
                    CSF_AT(std::uint8_t, context, 0x16) = 0;
                    ProcessRecord(record, nullptr, contextAddress);
                }
                ++index;
                offset += 16;
            }
        }
        else
        {
            const std::uint32_t capacity = CSF_AT(std::uint32_t, array, 0x4);
            CSF_AT(std::uint32_t, array, 0x8) = 0;
            if (count > capacity)
                GrowRecordArray(array, nullptr, static_cast<int>(count - capacity));

            std::uint32_t index = 0;
            std::uint32_t offset = 0;
            while (index < count)
            {
                if (CSF_AT(std::uint32_t, array, 0x8) >=
                    CSF_AT(std::uint32_t, array, 0x4))
                {
                    GrowRecordArray(
                        array, nullptr, CSF_AT(int, array, 0xC));
                }

                const std::uint32_t size = CSF_AT(std::uint32_t, array, 0x8);
                void* newRecord = reinterpret_cast<void*>(
                    CSF_AT(std::uint32_t, array, 0x0) + 16 * size);
                CSF_AT(std::uint32_t, array, 0x8) = size + 1;
                CSF_AT(std::uint32_t, newRecord, 0x0) = 0;
                CSF_AT(std::uint32_t, newRecord, 0x4) = 0;
                CSF_AT(std::uint32_t, newRecord, 0x8) = 0;
                CSF_AT(std::uint32_t, newRecord, 0xC) = 0;

                void* record = reinterpret_cast<void*>(
                    CSF_AT(std::uint32_t, array, 0x0) + offset);
                const bool canProcess =
                    CSF_AT(std::uint8_t, context, 0x0) == 0;
                CSF_AT(std::uint32_t, context, 0x1C) = 0;
                if (canProcess)
                {
                    CSF_AT(std::uint8_t, context, 0x15) = 0;
                    CSF_AT(std::uint8_t, context, 0x16) = 0;
                    ProcessRecord(record, nullptr, contextAddress);
                }
                offset += 16;
                ++index;
            }
        }

        if (CSF_AT(std::uint8_t, context, 0x0) == 0)
        {
            void* handler = CSF_AT(void*, context, 0x4);
            if (reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, handler, 0x0)[0x7C / 4])(
                        handler, nullptr, contextAddress))
            {
                CSF_AT(std::uint8_t, context, 0x0) = 0;
                return 0;
            }
        }
    }

    CSF_AT(std::uint8_t, context, 0x0) = 1;
    return 1;
}

HOOK(0x004443B0, ProcessRecordArray, "auto");
