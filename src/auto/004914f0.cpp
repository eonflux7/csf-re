#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Serialization_ProcessRecord, 0x0044ECA0,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Serialization_GrowRecordArray, 0x0048EAC0,
         (void* self, void* edx, int a2));

static int __stdcall Serialization_ProcessRecordArray(int* records, void* context)
{
    const uint8_t existingCountMode = CSF_AT(uint8_t, context, 0x14);
    const uint8_t preserveRecords = CSF_AT(uint8_t, context, 0x15);
    uint32_t recordCount = 0;
    if (existingCountMode)
        recordCount = CSF_AT(uint32_t, records, 0x08);

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* backend = CSF_AT(void*, context, 0x04);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, uint32_t*, void*)>(
            CSF_AT(void**, backend, 0x00)[0x78 / 4])(
                backend, nullptr, &recordCount, context);
    }
    CSF_AT(uint8_t, context, 0x00) = failed;

    if (!failed)
    {
        if (existingCountMode || preserveRecords)
        {
            uint32_t recordIndex = 0;
            uint32_t byteOffset = 0;
            while (recordIndex < recordCount)
            {
                void* record = CSF_AT(uint8_t*, records, 0x00) + byteOffset;
                const bool canProcess = CSF_AT(uint8_t, context, 0x00) == 0;
                CSF_AT(uint32_t, context, 0x1C) = 0;
                if (canProcess)
                {
                    CSF_AT(uint8_t, context, 0x15) = preserveRecords;
                    CSF_AT(uint8_t, context, 0x16) = 0;
                    Serialization_ProcessRecord(
                        record, nullptr,
                        static_cast<int>(reinterpret_cast<uintptr_t>(context)));
                }
                ++recordIndex;
                byteOffset += 12;
            }
        }
        else
        {
            const uint32_t capacity = CSF_AT(uint32_t, records, 0x04);
            CSF_AT(uint32_t, records, 0x08) = 0;
            if (recordCount > capacity)
            {
                Serialization_GrowRecordArray(
                    records, nullptr, static_cast<int>(recordCount - capacity));
            }

            uint32_t recordIndex = 0;
            uint32_t byteOffset = 0;
            while (recordIndex < recordCount)
            {
                if (CSF_AT(uint32_t, records, 0x08) >=
                    CSF_AT(uint32_t, records, 0x04))
                {
                    Serialization_GrowRecordArray(
                        records, nullptr, CSF_AT(int, records, 0x0C));
                }

                const uint32_t insertionIndex = CSF_AT(uint32_t, records, 0x08);
                uint8_t* storage = CSF_AT(uint8_t*, records, 0x00);
                CSF_AT(uint32_t, records, 0x08) = insertionIndex + 1;
                void* newRecord = storage + 12 * insertionIndex;
                CSF_AT(uint32_t, newRecord, 0x00) = 0;
                CSF_AT(uint32_t, newRecord, 0x04) = 0;
                CSF_AT(int, newRecord, 0x08) = -1;

                void* record = CSF_AT(uint8_t*, records, 0x00) + byteOffset;
                const bool canProcess = CSF_AT(uint8_t, context, 0x00) == 0;
                CSF_AT(uint32_t, context, 0x1C) = 0;
                if (canProcess)
                {
                    CSF_AT(uint8_t, context, 0x15) = 0;
                    CSF_AT(uint8_t, context, 0x16) = 0;
                    Serialization_ProcessRecord(
                        record, nullptr,
                        static_cast<int>(reinterpret_cast<uintptr_t>(context)));
                }
                ++recordIndex;
                byteOffset += 12;
            }
        }

        if (CSF_AT(uint8_t, context, 0x00) == 0)
        {
            void* backend = CSF_AT(void*, context, 0x04);
            if (reinterpret_cast<bool(__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, backend, 0x00)[0x7C / 4])(
                        backend, nullptr, context))
            {
                CSF_AT(uint8_t, context, 0x00) = 0;
                return 0;
            }
        }
    }

    CSF_AT(uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x004914F0, Serialization_ProcessRecordArray, "auto");
