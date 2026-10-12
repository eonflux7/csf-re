#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t*, __fastcall, RecordArray_CopyRecord, 0x0048A090, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, RecordArray_InitializeRecord, 0x0048A110, (void* self, void* edx));
CSF_FUNC(char*, __fastcall, RecordArray_Grow, 0x0048A2B0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, RecordArray_SerializeRecord, 0x0048A650, (void* self, void* edx, int a2));

static int __stdcall RecordArray_Serialize(void* array, void* context)
{
    const uint8_t existingCountMode = CSF_AT(uint8_t, context, 0x14);
    const uint8_t updateMode = CSF_AT(uint8_t, context, 0x15);
    uint32_t recordCount = 0;

    if (existingCountMode)
        recordCount = CSF_AT(uint32_t, array, 0x08);

    bool failed = false;
    if (!CSF_AT(uint8_t, context, 0x00))
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, uint32_t*, void*))
                       CSF_AT(void**, serializer, 0x00)[120 / 4])(
                           serializer, nullptr, &recordCount, context);
    }
    CSF_AT(uint8_t, context, 0x00) = failed;
    if (failed)
    {
        CSF_AT(uint8_t, context, 0x00) = 1;
        return 1;
    }

    if (existingCountMode || updateMode)
    {
        uint32_t index = 0;
        uint32_t byteOffset = 0;
        while (index < recordCount)
        {
            void* record = CSF_AT(uint8_t*, array, 0x00) + byteOffset;
            const bool contextFailed = CSF_AT(uint8_t, context, 0x00) != 0;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (!contextFailed)
            {
                CSF_AT(uint8_t, context, 0x15) = updateMode;
                CSF_AT(uint8_t, context, 0x16) = 0;
                RecordArray_SerializeRecord(record, nullptr, reinterpret_cast<int>(context));
            }
            ++index;
            byteOffset += 96;
        }
    }
    else
    {
        const uint32_t capacity = CSF_AT(uint32_t, array, 0x04);
        CSF_AT(uint32_t, array, 0x08) = 0;
        if (recordCount > capacity)
            RecordArray_Grow(array, nullptr, static_cast<int>(recordCount - capacity));

        alignas(8) uint8_t temporaryRecord[0x60];
        uint32_t index = 0;
        uint32_t byteOffset = 0;
        while (index < recordCount)
        {
            RecordArray_InitializeRecord(temporaryRecord, nullptr);
            if (CSF_AT(uint32_t, array, 0x08) >= CSF_AT(uint32_t, array, 0x04))
                RecordArray_Grow(array, nullptr, CSF_AT(int, array, 0x0C));

            const uint32_t size = CSF_AT(uint32_t, array, 0x08);
            void* destination = CSF_AT(uint8_t*, array, 0x00) + 96 * size;
            CSF_AT(uint32_t, array, 0x08) = size + 1;
            RecordArray_CopyRecord(destination, nullptr, reinterpret_cast<int>(temporaryRecord));

            void* record = CSF_AT(uint8_t*, array, 0x00) + byteOffset;
            const bool contextFailed = CSF_AT(uint8_t, context, 0x00) != 0;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (!contextFailed)
            {
                CSF_AT(uint8_t, context, 0x15) = 0;
                CSF_AT(uint8_t, context, 0x16) = 0;
                RecordArray_SerializeRecord(record, nullptr, reinterpret_cast<int>(context));
            }

            CSF_AT(void**, temporaryRecord, 0x3C) = CSF_VTBL(0x0082D410);
            CSF_AT(void**, temporaryRecord, 0x2C) = CSF_VTBL(0x0082D410);
            CSF_AT(void**, temporaryRecord, 0x10) = CSF_VTBL(0x0082D410);
            byteOffset += 96;
            ++index;
        }
    }

    if (!CSF_AT(uint8_t, context, 0x00))
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        if (((bool(__fastcall*)(void*, void*, void*))
                 CSF_AT(void**, serializer, 0x00)[124 / 4])(
                     serializer, nullptr, context))
        {
            CSF_AT(uint8_t, context, 0x00) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x0048A7B0, RecordArray_Serialize, "auto");
