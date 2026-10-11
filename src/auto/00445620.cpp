#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, S_00413570_GrowEventRecords, 0x00413570, (S_00413570* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));

[[maybe_unused]] static const auto g_EventRecordsVtable = CSF_VTBL(0x00413570);

static int __stdcall SceneLoader_SerializeRecordMember(
    S_00413570* records, int archiveAddress)
{
    void* archive = reinterpret_cast<void*>(archiveAddress);
    const uint8_t writing = CSF_AT(uint8_t, archive, 0x14);
    const uint8_t existingRecords = CSF_AT(uint8_t, archive, 0x15);

    // The read path originally passes an uninitialized count.
    uint32_t count = 0;
    if (writing)
        count = records->field_0008;

    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        failed = !reinterpret_cast<bool (__fastcall*)(
            void*, void*, uint32_t*, int)>(
                CSF_AT(void**, serializer, 0)[0x78 / 4])(
                    serializer, nullptr, &count, archiveAddress);
    }
    CSF_AT(uint8_t, archive, 0x00) = failed;

    if (!failed)
    {
        if (writing || existingRecords)
        {
            uint32_t index = 0;
            uint32_t offset = 0;
            if (count)
            {
                do
                {
                    const uint32_t recordAddress = records->field_0000 + offset;
                    const bool ready = CSF_AT(uint8_t, archive, 0x00) == 0;
                    CSF_AT(uint32_t, archive, 0x1C) = 0;
                    if (ready)
                    {
                        CSF_AT(uint8_t, archive, 0x15) = existingRecords;
                        CSF_AT(uint8_t, archive, 0x16) = 0;
                        Archive_SerializeString(
                            static_cast<int>(recordAddress),
                            reinterpret_cast<S_00691860*>(archive));
                    }
                    ++index;
                    offset += 12;
                } while (index < count);
            }
        }
        else
        {
            const uint32_t capacity = records->field_0004;
            uint32_t requestedCount = count;
            records->field_0008 = 0;
            if (requestedCount > capacity)
            {
                S_00413570_GrowEventRecords(
                    records, nullptr, static_cast<int>(requestedCount - capacity));
                requestedCount = count;
            }

            uint32_t index = 0;
            uint32_t offset = 0;
            if (requestedCount)
            {
                do
                {
                    if (records->field_0008 >= records->field_0004)
                    {
                        S_00413570_GrowEventRecords(
                            records, nullptr, static_cast<int>(records->field_000c));
                    }

                    const uint32_t oldCount = records->field_0008;
                    const uint32_t dataAddress = records->field_0000;
                    records->field_0008 = oldCount + 1;
                    void* newRecord = reinterpret_cast<void*>(dataAddress + 12 * oldCount);
                    CSF_AT(int32_t, newRecord, 0x04) = -1;
                    CSF_AT(int32_t, newRecord, 0x08) = -1;

                    const uint32_t recordAddress = records->field_0000 + offset;
                    const bool ready = CSF_AT(uint8_t, archive, 0x00) == 0;
                    CSF_AT(uint32_t, archive, 0x1C) = 0;
                    if (ready)
                    {
                        CSF_AT(uint8_t, archive, 0x15) = 0;
                        CSF_AT(uint8_t, archive, 0x16) = 0;
                        Archive_SerializeString(
                            static_cast<int>(recordAddress),
                            reinterpret_cast<S_00691860*>(archive));
                    }
                    offset += 12;
                    ++index;
                } while (index < count);
            }
        }

        if (CSF_AT(uint8_t, archive, 0x00) == 0)
        {
            void* serializer = CSF_AT(void*, archive, 0x04);
            if (reinterpret_cast<bool (__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, serializer, 0)[0x7C / 4])(
                        serializer, nullptr, archiveAddress))
            {
                CSF_AT(uint8_t, archive, 0x00) = 0;
                return 0;
            }
        }
    }

    CSF_AT(uint8_t, archive, 0x00) = 1;
    return 1;
}

HOOK(0x00445620, SceneLoader_SerializeRecordMember, "auto");
