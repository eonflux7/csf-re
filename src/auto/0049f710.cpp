#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, Archive_GrowRecordArray, 0x0049F330,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Archive_SerializeRecord, 0x0049F600,
         (void* self, void* edx, void* a2));

static unsigned int __stdcall Archive_SerializeRecordArray(
    void* array, void* context)
{
    const uint8_t savedMode = CSF_AT(uint8_t, context, 0x15);
    const uint8_t preserveCount = CSF_AT(uint8_t, context, 0x14);
    const uint8_t initialError = CSF_AT(uint8_t, context, 0);

    alignas(8) uint8_t countBytes[4] = {};
    uint32_t& count = *reinterpret_cast<uint32_t*>(countBytes);
    if (preserveCount)
        count = CSF_AT(uint32_t, array, 0x08);

    unsigned int result = 0;
    if (!initialError)
    {
        void* archive = reinterpret_cast<void*>(
            CSF_AT(uint32_t, context, 0x04));
        const bool success =
            ((bool(__fastcall*)(void*, void*, uint32_t*, void*))
                 CSF_AT(void**, archive, 0)[120 / 4])(
                archive, nullptr, &count, context);
        result = !success;
    }

    CSF_AT(uint8_t, context, 0) = static_cast<uint8_t>(result);
    if (result)
    {
        CSF_AT(uint8_t, context, 0) = 1;
        return result;
    }

    if (preserveCount || savedMode)
    {
        uint32_t index = 0;
        uint32_t offset = 0;
        while (index < count)
        {
            void* record = reinterpret_cast<uint8_t*>(
                CSF_AT(uint32_t, array, 0)) + offset;
            const bool hasError = CSF_AT(uint8_t, context, 0) != 0;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (!hasError)
            {
                CSF_AT(uint8_t, context, 0x15) = savedMode;
                CSF_AT(uint8_t, context, 0x16) = 0;
                Archive_SerializeRecord(record, nullptr, context);
            }
            result = ++index;
            offset += 28;
        }
    }
    else
    {
        result = CSF_AT(uint32_t, array, 0x04);
        CSF_AT(uint32_t, array, 0x08) = 0;
        if (count > result)
        {
            result = reinterpret_cast<uintptr_t>(
                Archive_GrowRecordArray(
                    array, nullptr, static_cast<int>(count - result)));
        }

        uint32_t index = 0;
        uint32_t offset = 0;
        while (index < count)
        {
            if (CSF_AT(uint32_t, array, 0x08) >=
                CSF_AT(uint32_t, array, 0x04))
            {
                Archive_GrowRecordArray(
                    array, nullptr, CSF_AT(int, array, 0x0C));
            }

            const uint32_t used = CSF_AT(uint32_t, array, 0x08);
            uint8_t* record = reinterpret_cast<uint8_t*>(
                CSF_AT(uint32_t, array, 0)) + 28 * used;
            CSF_AT(uint32_t, array, 0x08) = used + 1;
            CSF_AT(int, record, 0x04) = -1;
            CSF_AT(int, record, 0x08) = -1;
            CSF_AT(int, record, 0x0C) = 0;
            CSF_AT(int, record, 0x10) = 0;
            CSF_AT(int, record, 0x14) = 0;
            CSF_AT(uint32_t, record, 0x18) = 0;

            void* currentRecord = reinterpret_cast<uint8_t*>(
                CSF_AT(uint32_t, array, 0)) + offset;
            const bool hasError = CSF_AT(uint8_t, context, 0) != 0;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (!hasError)
            {
                CSF_AT(uint8_t, context, 0x15) = 0;
                CSF_AT(uint8_t, context, 0x16) = 0;
                Archive_SerializeRecord(currentRecord, nullptr, context);
            }
            result = ++index;
            offset += 28;
        }
    }

    uint8_t error = 1;
    if (!CSF_AT(uint8_t, context, 0))
    {
        void* archive = reinterpret_cast<void*>(
            CSF_AT(uint32_t, context, 0x04));
        result = ((int(__fastcall*)(void*, void*, void*))
                      CSF_AT(void**, archive, 0)[124 / 4])(
            archive, nullptr, context);
        if (static_cast<uint8_t>(result))
            error = 0;
    }
    CSF_AT(uint8_t, context, 0) = error;
    return result;
}

HOOK(0x0049F710, Archive_SerializeRecordArray, "auto");
