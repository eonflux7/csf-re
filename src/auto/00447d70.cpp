#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, RecordArray_GrowStorage, 0x00445A80,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Record_Serialize, 0x004475B0,
         (void* self, void* edx, int a2));

static int __stdcall SerializeRecordArray(void* array, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    const uint8_t savedElementMode = CSF_AT(uint8_t, context, 0x15);
    const bool existingArrayMode = CSF_AT(uint8_t, context, 0x14) != 0;
    const uint8_t previousError = CSF_AT(uint8_t, context, 0);

    alignas(8) uint8_t countBytes[0x4];
    uint32_t& count = *reinterpret_cast<uint32_t*>(countBytes);
    count = existingArrayMode ? CSF_AT(uint32_t, array, 0x8) : 0;

    bool failed = false;
    if (!previousError)
    {
        void* serializer = CSF_AT(void*, context, 0x4);
        failed = !((bool(__fastcall*)(void*, void*, uint32_t*, int))
                       CSF_AT(void**, serializer, 0)[0x78 / 4])(
            serializer, nullptr, &count, contextAddress);
    }

    CSF_AT(uint8_t, context, 0) = failed;
    if (failed)
    {
        CSF_AT(uint8_t, context, 0) = 1;
        return 1;
    }

    if (existingArrayMode || savedElementMode)
    {
        uint32_t index = 0;
        uint32_t offset = 0;
        while (index < count)
        {
            void* element = CSF_AT(uint8_t*, array, 0) + offset;
            const bool canSerialize = CSF_AT(uint8_t, context, 0) == 0;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (canSerialize)
            {
                CSF_AT(uint8_t, context, 0x15) = savedElementMode;
                CSF_AT(uint8_t, context, 0x16) = 0;
                Record_Serialize(element, nullptr, contextAddress);
            }
            ++index;
            offset += 32;
        }
    }
    else
    {
        const uint32_t capacity = CSF_AT(uint32_t, array, 0x4);
        uint32_t requestedCount = count;
        CSF_AT(uint32_t, array, 0x8) = 0;
        if (requestedCount > capacity)
        {
            RecordArray_GrowStorage(array, nullptr,
                                    static_cast<int>(requestedCount - capacity));
            requestedCount = count;
        }

        if (requestedCount)
        {
            alignas(8) uint8_t defaultRecord[0x20];
            std::memset(defaultRecord + 4, 0, 16);
            CSF_AT(int32_t, defaultRecord, 0) = -1;
            CSF_AT(float, defaultRecord, 0x14) = 1.0f;
            CSF_AT(uint32_t, defaultRecord, 0x18) = 0;
            CSF_AT(uint32_t, defaultRecord, 0x1C) = 0;

            uint32_t index = 0;
            uint32_t offset = 0;
            do
            {
                if (CSF_AT(uint32_t, array, 0x8) >=
                    CSF_AT(uint32_t, array, 0x4))
                {
                    RecordArray_GrowStorage(
                        array, nullptr,
                        CSF_AT(int, array, 0xC));
                }

                const uint32_t appendIndex = CSF_AT(uint32_t, array, 0x8);
                void* destination =
                    CSF_AT(uint8_t*, array, 0) + 32 * appendIndex;
                CSF_AT(uint32_t, array, 0x8) = appendIndex + 1;
                std::memcpy(destination, defaultRecord, 0x20);

                void* element = CSF_AT(uint8_t*, array, 0) + offset;
                const bool canSerialize = CSF_AT(uint8_t, context, 0) == 0;
                CSF_AT(uint32_t, context, 0x1C) = 0;
                if (canSerialize)
                {
                    CSF_AT(uint8_t, context, 0x15) = 0;
                    CSF_AT(uint8_t, context, 0x16) = 0;
                    Record_Serialize(element, nullptr, contextAddress);
                }

                ++index;
                offset += 32;
            } while (index < count);
        }
    }

    if (!CSF_AT(uint8_t, context, 0))
    {
        void* serializer = CSF_AT(void*, context, 0x4);
        if (((bool(__fastcall*)(void*, void*, int))
                 CSF_AT(void**, serializer, 0)[0x7C / 4])(
                serializer, nullptr, contextAddress))
        {
            CSF_AT(uint8_t, context, 0) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x00447D70, SerializeRecordArray, "auto");
