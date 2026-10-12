#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, Archive_GrowRecordArray, 0x00448C60,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Archive_SerializeRecord, 0x00448F30,
         (void* self, void* edx, void* a2));

static int __stdcall Archive_SerializeRecordArray(
    void* records, S_00691860* context)
{
    const uint8_t writing =
        reinterpret_cast<uint8_t*>(&context->field_0014)[0];
    const uint8_t existingOnly = CSF_AT(uint8_t, context, 0x15);

    alignas(8) uint8_t countBytes[0x4] = {};
    uint32_t& count = *reinterpret_cast<uint32_t*>(countBytes);

    if (writing)
        count = CSF_AT(uint32_t, records, 0x08);

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* archive = reinterpret_cast<void*>(context->field_0004);
        failed =
            !((bool(__fastcall*)(void*, void*, uint32_t*, S_00691860*))
                  CSF_AT(void**, archive, 0)[0x78 / 4])(
                archive, nullptr, &count, context);
    }
    CSF_AT(uint8_t, context, 0x00) = static_cast<uint8_t>(failed);

    if (!failed)
    {
        if (writing || existingOnly)
        {
            uint32_t index = 0;
            uint32_t offset = 0;
            while (index < count)
            {
                void* record = CSF_AT(uint8_t*, records, 0x00) + offset;
                const bool canSerialize =
                    CSF_AT(uint8_t, context, 0x00) == 0;
                CSF_AT(uint32_t, context, 0x1C) = 0;
                if (canSerialize)
                {
                    CSF_AT(uint8_t, context, 0x15) = existingOnly;
                    CSF_AT(uint8_t, context, 0x16) = 0;
                    Archive_SerializeRecord(record, nullptr, context);
                }
                ++index;
                offset += 36;
            }
        }
        else
        {
            const uint32_t capacity = CSF_AT(uint32_t, records, 0x04);
            CSF_AT(uint32_t, records, 0x08) = 0;
            if (count > capacity)
            {
                Archive_GrowRecordArray(
                    records, nullptr, static_cast<int>(count - capacity));
            }

            uint32_t index = 0;
            uint32_t offset = 0;
            while (index < count)
            {
                if (CSF_AT(uint32_t, records, 0x08) >=
                    CSF_AT(uint32_t, records, 0x04))
                {
                    Archive_GrowRecordArray(
                        records, nullptr,
                        static_cast<int>(CSF_AT(uint32_t, records, 0x0C)));
                }

                const uint32_t used = CSF_AT(uint32_t, records, 0x08);
                uint8_t* record =
                    CSF_AT(uint8_t*, records, 0x00) + 36 * used;
                CSF_AT(uint32_t, records, 0x08) = used + 1;
                CSF_AT(int32_t, record, 0x04) = -1;
                CSF_AT(int32_t, record, 0x08) = -1;
                CSF_AT(uint32_t, record, 0x0C) = 0;
                CSF_AT(int32_t, record, 0x14) = -1;
                CSF_AT(int32_t, record, 0x18) = -1;
                CSF_AT(uint32_t, record, 0x1C) = 0;
                CSF_AT(uint32_t, record, 0x20) = 0;

                void* serializedRecord =
                    CSF_AT(uint8_t*, records, 0x00) + offset;
                const bool canSerialize =
                    CSF_AT(uint8_t, context, 0x00) == 0;
                CSF_AT(uint32_t, context, 0x1C) = 0;
                if (canSerialize)
                {
                    CSF_AT(uint8_t, context, 0x15) = 0;
                    CSF_AT(uint8_t, context, 0x16) = 0;
                    Archive_SerializeRecord(serializedRecord, nullptr, context);
                }
                ++index;
                offset += 36;
            }
        }

        if (CSF_AT(uint8_t, context, 0x00) == 0)
        {
            void* archive = reinterpret_cast<void*>(context->field_0004);
            if (((bool(__fastcall*)(void*, void*, S_00691860*))
                     CSF_AT(void**, archive, 0)[0x7C / 4])(
                    archive, nullptr, context))
            {
                CSF_AT(uint8_t, context, 0x00) = 0;
                return 0;
            }
        }
    }

    CSF_AT(uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x00449E30, Archive_SerializeRecordArray, "auto");
