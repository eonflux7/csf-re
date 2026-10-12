#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, RecordArray_Grow, 0x00425CE0,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Record_Serialize, 0x00426EC0,
         (void* self, void* edx, int a2));

static int __stdcall RecordArray_Serialize(int arrayAddress, int contextAddress)
{
    auto* array = reinterpret_cast<uint8_t*>(arrayAddress);
    auto* context = reinterpret_cast<uint8_t*>(contextAddress);

    const uint8_t savedMode = CSF_AT(uint8_t, context, 0x15);
    const uint8_t initialError = CSF_AT(uint8_t, context, 0);
    const bool useExistingRecords = CSF_AT(uint8_t, context, 0x14) != 0;

    alignas(8) uint8_t countBytes[0x4] = {};
    auto& count = *reinterpret_cast<uint32_t*>(countBytes);
    if (useExistingRecords)
        count = CSF_AT(uint32_t, array, 0x8);

    bool failed = false;
    if (!initialError)
    {
        void* stream = CSF_AT(void*, context, 0x4);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, uint32_t*, int)>(
            CSF_AT(void**, stream, 0)[120 / 4])(
                stream, nullptr, &count, contextAddress);
    }
    CSF_AT(uint8_t, context, 0) = failed;

    if (failed)
    {
        CSF_AT(uint8_t, context, 0) = 1;
        return 1;
    }

    if (useExistingRecords || savedMode)
    {
        uint32_t index = 0;
        uint32_t recordOffset = 0;
        while (index < count)
        {
            void* record = CSF_AT(uint8_t*, array, 0) + recordOffset;
            const bool canSerialize = CSF_AT(uint8_t, context, 0) == 0;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (canSerialize)
            {
                CSF_AT(uint8_t, context, 0x15) = savedMode;
                CSF_AT(uint8_t, context, 0x16) = 0;
                Record_Serialize(record, nullptr, contextAddress);
            }
            ++index;
            recordOffset += 36;
        }
    }
    else
    {
        const uint32_t capacity = CSF_AT(uint32_t, array, 0x4);
        const uint32_t requestedCount = count;
        CSF_AT(uint32_t, array, 0x8) = 0;
        if (requestedCount > capacity)
            RecordArray_Grow(array, nullptr,
                             static_cast<int>(requestedCount - capacity));

        if (count)
        {
            alignas(8) uint8_t defaultRecordBytes[0x24];
            CSF_AT(uint32_t, defaultRecordBytes, 0) = 1;
            std::memset(defaultRecordBytes + 4, 0, 29);
            // The original copies three uninitialized trailing bytes.
            std::memset(defaultRecordBytes + 33, 0, 3);

            uint32_t index = 0;
            uint32_t recordOffset = 0;
            do
            {
                if (CSF_AT(uint32_t, array, 0x8) >=
                    CSF_AT(uint32_t, array, 0x4))
                {
                    RecordArray_Grow(array, nullptr,
                                     CSF_AT(int, array, 0xC));
                }

                const uint32_t size = CSF_AT(uint32_t, array, 0x8);
                void* destination = CSF_AT(uint8_t*, array, 0) + 36 * size;
                CSF_AT(uint32_t, array, 0x8) = size + 1;
                std::memcpy(destination, defaultRecordBytes, 0x24);

                void* record = CSF_AT(uint8_t*, array, 0) + recordOffset;
                const bool canSerialize = CSF_AT(uint8_t, context, 0) == 0;
                CSF_AT(uint32_t, context, 0x1C) = 0;
                if (canSerialize)
                {
                    CSF_AT(uint8_t, context, 0x15) = 0;
                    CSF_AT(uint8_t, context, 0x16) = 0;
                    Record_Serialize(record, nullptr, contextAddress);
                }

                ++index;
                recordOffset += 36;
            }
            while (index < count);
        }
    }

    if (!CSF_AT(uint8_t, context, 0))
    {
        void* stream = CSF_AT(void*, context, 0x4);
        if (reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, stream, 0)[124 / 4])(
                    stream, nullptr, contextAddress))
        {
            CSF_AT(uint8_t, context, 0) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x00427020, RecordArray_Serialize, "auto");
