#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, RecordStorage_SerializeRecord, 0x006B5610, (void* a1, int a2));
CSF_FUNC(std::uint32_t, __fastcall, ClearRecordStorage_6B59F0, 0x006B59F0, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, RecordStorage_ReserveRecords, 0x006B5D80, (void* self, void* edx, unsigned int a2));
CSF_FUNC(char, __fastcall, RecordStorage_AppendRecord, 0x006B5F20, (void* self, void* edx, int a3));

static int __stdcall S_006b59f0_SerializeRecords(
    S_006b59f0* self, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    const std::uint8_t writing = CSF_AT(std::uint8_t, context, 0x14);
    const std::uint8_t existingRecords = CSF_AT(std::uint8_t, context, 0x15);
    unsigned int count = 0;

    if (writing)
    {
        const std::uint32_t begin = self->field_0004;
        unsigned int limit = begin
            ? static_cast<std::int32_t>(self->field_0008 - begin) >> 4
            : 0;
        count = limit;

        bool failed = false;
        if (CSF_AT(std::uint8_t, context, 0x00) == 0)
        {
            void* serializer = CSF_AT(void*, context, 0x04);
            failed = !((bool(__fastcall*)(void*, void*, unsigned int*, int))
                CSF_AT(void**, serializer, 0)[0x78 / 4])(
                    serializer, nullptr, &count, contextAddress);
            limit = count;
        }
        CSF_AT(std::uint8_t, context, 0x00) = failed;
        if (failed)
        {
            CSF_AT(std::uint8_t, context, 0x00) = 1;
            return 1;
        }

        unsigned int index = 0;
        std::uint32_t offset = 0;
        while (index < limit)
        {
            void* record = reinterpret_cast<void*>(self->field_0004 + offset);
            const bool canSerialize = CSF_AT(std::uint8_t, context, 0x00) == 0;
            CSF_AT(std::uint32_t, context, 0x1C) = 0;
            if (canSerialize)
            {
                CSF_AT(std::uint8_t, context, 0x15) = existingRecords;
                CSF_AT(std::uint8_t, context, 0x16) = 0;
                RecordStorage_SerializeRecord(record, contextAddress);
                limit = count;
            }
            ++index;
            offset += 16;
        }
    }
    else
    {
        bool failed = false;
        if (CSF_AT(std::uint8_t, context, 0x00) == 0)
        {
            void* serializer = CSF_AT(void*, context, 0x04);
            failed = !((bool(__fastcall*)(void*, void*, unsigned int*, int))
                CSF_AT(void**, serializer, 0)[0x78 / 4])(
                    serializer, nullptr, &count, contextAddress);
        }
        CSF_AT(std::uint8_t, context, 0x00) = failed;
        if (failed)
        {
            CSF_AT(std::uint8_t, context, 0x00) = 1;
            return 1;
        }

        if (existingRecords)
        {
            unsigned int index = 0;
            std::uint32_t offset = 0;
            while (index < count)
            {
                void* record = reinterpret_cast<void*>(self->field_0004 + offset);
                const bool canSerialize = CSF_AT(std::uint8_t, context, 0x00) == 0;
                CSF_AT(std::uint32_t, context, 0x1C) = 0;
                if (canSerialize)
                {
                    CSF_AT(std::uint8_t, context, 0x15) = existingRecords;
                    CSF_AT(std::uint8_t, context, 0x16) = 0;
                    RecordStorage_SerializeRecord(record, contextAddress);
                }
                ++index;
                offset += 16;
            }
        }
        else
        {
            // The packet also lists this callable address as a vtable reference.
            (void)CSF_VTBL(0x006B59F0);
            ClearRecordStorage_6B59F0(self, nullptr);
            RecordStorage_ReserveRecords(self, nullptr, count);

            if (count)
            {
                alignas(8) std::uint8_t recordBytes[0x10] = {};
                unsigned int index = 0;
                std::uint32_t offset = 0;
                do
                {
                    RecordStorage_AppendRecord(
                        self, nullptr, reinterpret_cast<int>(recordBytes));
                    void* record = reinterpret_cast<void*>(self->field_0004 + offset);
                    const bool canSerialize = CSF_AT(std::uint8_t, context, 0x00) == 0;
                    CSF_AT(std::uint32_t, context, 0x1C) = 0;
                    if (canSerialize)
                    {
                        CSF_AT(std::uint8_t, context, 0x15) = 0;
                        CSF_AT(std::uint8_t, context, 0x16) = 0;
                        RecordStorage_SerializeRecord(record, contextAddress);
                    }
                    offset += 16;
                    ++index;
                } while (index < count);
            }
        }
    }

    if (CSF_AT(std::uint8_t, context, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        if (((bool(__fastcall*)(void*, void*, int))
                CSF_AT(void**, serializer, 0)[0x7C / 4])(
                    serializer, nullptr, contextAddress))
        {
            CSF_AT(std::uint8_t, context, 0x00) = 0;
            return 0;
        }
    }

    CSF_AT(std::uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x006B5F90, S_006b59f0_SerializeRecords, "auto");
