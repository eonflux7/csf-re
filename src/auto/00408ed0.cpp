#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, ProcessRecord, 0x00408BB0,
         (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GrowRecordArray, 0x006D9EB0,
         (void* self, void* edx, int a2));

static int __stdcall ProcessRecordArray(int* records, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    const uint8_t useExistingCount = CSF_AT(uint8_t, context, 0x14);
    const uint8_t updateExisting = CSF_AT(uint8_t, context, 0x15);

    uint32_t recordCount = 0;
    if (useExistingCount)
        recordCount = CSF_AT(uint32_t, records, 0x08);

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* handler = reinterpret_cast<void*>(
            CSF_AT(uint32_t, context, 0x04));
        failed = !reinterpret_cast<bool(__fastcall*)(
            void*, void*, uint32_t*, int)>(
                CSF_AT(void**, handler, 0x00)[0x78 / 4])(
                    handler, nullptr, &recordCount, contextAddress);
    }
    CSF_AT(uint8_t, context, 0x00) = static_cast<uint8_t>(failed);

    if (!failed)
    {
        if (useExistingCount || updateExisting)
        {
            uint32_t index = 0;
            uint32_t offset = 0;
            if (recordCount)
            {
                do
                {
                    void* record = reinterpret_cast<void*>(
                        CSF_AT(uint32_t, records, 0x00) + offset);
                    const bool process = CSF_AT(uint8_t, context, 0x00) == 0;
                    CSF_AT(uint32_t, context, 0x1C) = 0;
                    if (process)
                    {
                        CSF_AT(uint8_t, context, 0x15) = updateExisting;
                        CSF_AT(uint8_t, context, 0x16) = 0;
                        ProcessRecord(record, nullptr, contextAddress);
                    }
                    ++index;
                    offset += 12;
                } while (index < recordCount);
            }
        }
        else
        {
            const uint32_t capacity = CSF_AT(uint32_t, records, 0x04);
            CSF_AT(uint32_t, records, 0x08) = 0;
            if (recordCount > capacity)
                GrowRecordArray(records, nullptr,
                                static_cast<int>(recordCount - capacity));

            uint32_t index = 0;
            uint32_t offset = 0;
            if (recordCount)
            {
                do
                {
                    if (CSF_AT(uint32_t, records, 0x08) >=
                        CSF_AT(uint32_t, records, 0x04))
                    {
                        GrowRecordArray(records, nullptr,
                            CSF_AT(int, records, 0x0C));
                    }

                    const uint32_t count = CSF_AT(uint32_t, records, 0x08);
                    const uint32_t data = CSF_AT(uint32_t, records, 0x00);
                    CSF_AT(uint32_t, records, 0x08) = count + 1;
                    void* appendedRecord =
                        reinterpret_cast<void*>(data + 12 * count);
                    CSF_AT(uint32_t, appendedRecord, 0x00) = 0;
                    CSF_AT(uint32_t, appendedRecord, 0x04) = 0;
                    CSF_AT(uint32_t, appendedRecord, 0x08) = 0;

                    void* record = reinterpret_cast<void*>(
                        CSF_AT(uint32_t, records, 0x00) + offset);
                    const bool process = CSF_AT(uint8_t, context, 0x00) == 0;
                    CSF_AT(uint32_t, context, 0x1C) = 0;
                    if (process)
                    {
                        CSF_AT(uint8_t, context, 0x15) = 0;
                        CSF_AT(uint8_t, context, 0x16) = 0;
                        ProcessRecord(record, nullptr, contextAddress);
                    }

                    ++index;
                    offset += 12;
                } while (index < recordCount);
            }
        }

        if (CSF_AT(uint8_t, context, 0x00) == 0)
        {
            void* handler = reinterpret_cast<void*>(
                CSF_AT(uint32_t, context, 0x04));
            if (reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, handler, 0x00)[0x7C / 4])(
                        handler, nullptr, contextAddress))
            {
                CSF_AT(uint8_t, context, 0x00) = 0;
                return 0;
            }
        }
    }

    CSF_AT(uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x00408ED0, ProcessRecordArray, "auto");
