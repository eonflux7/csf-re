#include <cstdint>
#include <cstring>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, Array_GrowRecords, 0x00406EC0,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Array_ProcessRecord, 0x004078D0,
         (void* self, void* edx, int a2));

static int __stdcall Array_ProcessRecords(void* records, void* context)
{
    const uint8_t savedMode = CSF_AT(uint8_t, context, 0x15);
    const bool inPlace = CSF_AT(uint8_t, context, 0x14) != 0;
    const uint8_t initialStatus = CSF_AT(uint8_t, context, 0);
    uint32_t count = 0;

    if (inPlace)
        count = CSF_AT(uint32_t, records, 0x08);

    bool failed = false;
    if (initialStatus == 0)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        failed = !reinterpret_cast<bool (__fastcall*)(
            void*, void*, uint32_t*, void*)>(
                CSF_AT(void**, processor, 0)[0x78 / 4])(
                    processor, nullptr, &count, context);
    }
    CSF_AT(uint8_t, context, 0) = static_cast<uint8_t>(failed);

    if (failed)
    {
        CSF_AT(uint8_t, context, 0) = 1;
        return 1;
    }

    if (inPlace || savedMode != 0)
    {
        uint32_t offset = 0;
        for (uint32_t index = 0; index < count; ++index, offset += 44)
        {
            void* record = CSF_AT(uint8_t*, records, 0) + offset;
            const bool process = CSF_AT(uint8_t, context, 0) == 0;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (process)
            {
                CSF_AT(uint8_t, context, 0x15) = savedMode;
                CSF_AT(uint8_t, context, 0x16) = 0;
                Array_ProcessRecord(record, nullptr, reinterpret_cast<int>(context));
            }
        }
    }
    else
    {
        const uint32_t capacity = CSF_AT(uint32_t, records, 0x04);
        CSF_AT(uint32_t, records, 0x08) = 0;
        if (count > capacity)
            Array_GrowRecords(records, nullptr, static_cast<int>(count - capacity));

        if (count != 0)
        {
            uint8_t defaultRecord[0x2C] = {};
            CSF_AT(int32_t, defaultRecord, 0x08) = -1;
            CSF_AT(float, defaultRecord, 0x0C) = FLT_MAX;
            CSF_AT(float, defaultRecord, 0x10) = FLT_MAX;
            CSF_AT(float, defaultRecord, 0x14) = FLT_MAX;
            CSF_AT(float, defaultRecord, 0x1C) = FLT_MAX;
            CSF_AT(float, defaultRecord, 0x20) = FLT_MAX;
            CSF_AT(float, defaultRecord, 0x24) = FLT_MAX;

            uint32_t index = 0;
            uint32_t offset = 0;
            do
            {
                if (CSF_AT(uint32_t, records, 0x08) >=
                    CSF_AT(uint32_t, records, 0x04))
                {
                    Array_GrowRecords(
                        records, nullptr,
                        static_cast<int>(CSF_AT(uint32_t, records, 0x0C)));
                }

                const uint32_t size = CSF_AT(uint32_t, records, 0x08);
                void* destination = CSF_AT(uint8_t*, records, 0) + 44 * size;
                CSF_AT(uint32_t, records, 0x08) = size + 1;
                std::memcpy(destination, defaultRecord, 0x2C);

                void* record = CSF_AT(uint8_t*, records, 0) + offset;
                const bool process = CSF_AT(uint8_t, context, 0) == 0;
                CSF_AT(uint32_t, context, 0x1C) = 0;
                if (process)
                {
                    CSF_AT(uint8_t, context, 0x15) = 0;
                    CSF_AT(uint8_t, context, 0x16) = 0;
                    Array_ProcessRecord(record, nullptr, reinterpret_cast<int>(context));
                }

                ++index;
                offset += 44;
            } while (index < count);
        }
    }

    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        if (reinterpret_cast<bool (__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, processor, 0)[0x7C / 4])(
                    processor, nullptr, context))
        {
            CSF_AT(uint8_t, context, 0) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x00407BA0, Array_ProcessRecords, "auto");
