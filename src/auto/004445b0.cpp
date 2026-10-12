#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, GrowRecordArray, 0x00443E00,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ProcessRecord, 0x004442D0,
         (void* self, void* edx, void* a2));

static uint32_t __stdcall ProcessRecordArray(void* array, char* context)
{
    const char savedMode = CSF_AT(char, context, 0x15);
    const bool useExistingCount = CSF_AT(char, context, 0x14) != 0;
    const char initialError = CSF_AT(char, context, 0);

    alignas(8) uint8_t countBytes[4];
    uint32_t& count = *reinterpret_cast<uint32_t*>(countBytes);
    count = 0;
    if (useExistingCount)
        count = CSF_AT(uint32_t, array, 0x08);

    uint32_t result = 0;
    if (!initialError)
    {
        void* stream = CSF_AT(void*, context, 0x04);
        const bool succeeded =
            ((bool(__fastcall*)(void*, void*, uint32_t*, char*))
                 CSF_AT(void**, stream, 0)[0x78 / 4])(
                stream, nullptr, &count, context);
        result = !succeeded;
    }

    CSF_AT(char, context, 0) = static_cast<char>(result);
    if (result)
    {
        CSF_AT(char, context, 0) = 1;
        return result;
    }

    if (useExistingCount || savedMode)
    {
        uint32_t offset = 0;
        for (uint32_t index = 0; index < count; )
        {
            void* record = CSF_AT(uint8_t*, array, 0) + offset;
            const bool hasError = CSF_AT(char, context, 0) != 0;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (!hasError)
            {
                CSF_AT(char, context, 0x15) = savedMode;
                CSF_AT(char, context, 0x16) = 0;
                ProcessRecord(record, nullptr, context);
            }
            result = ++index;
            offset += 40;
        }
    }
    else
    {
        result = CSF_AT(uint32_t, array, 0x04);
        CSF_AT(uint32_t, array, 0x08) = 0;
        if (count > result)
        {
            result = reinterpret_cast<uint32_t>(
                GrowRecordArray(array, nullptr, static_cast<int>(count - result)));
        }

        uint32_t offset = 0;
        for (uint32_t index = 0; index < count; )
        {
            if (CSF_AT(uint32_t, array, 0x08) >=
                CSF_AT(uint32_t, array, 0x04))
            {
                GrowRecordArray(array, nullptr, CSF_AT(int, array, 0x0C));
            }

            const uint32_t size = CSF_AT(uint32_t, array, 0x08);
            void* addedRecord = CSF_AT(uint8_t*, array, 0) + 40 * size;
            CSF_AT(uint32_t, array, 0x08) = size + 1;
            CSF_AT(char, addedRecord, 0) = 0;
            CSF_AT(int, addedRecord, 0x08) = -1;
            CSF_AT(int, addedRecord, 0x0C) = -1;
            CSF_AT(uint32_t, addedRecord, 0x10) = 0;
            CSF_AT(uint32_t, addedRecord, 0x14) = 0;
            CSF_AT(uint32_t, addedRecord, 0x18) = 0;
            CSF_AT(uint32_t, addedRecord, 0x1C) = 0;
            CSF_AT(uint32_t, addedRecord, 0x20) = 0;
            CSF_AT(uint32_t, addedRecord, 0x24) = 0;

            void* record = CSF_AT(uint8_t*, array, 0) + offset;
            const bool hasError = CSF_AT(char, context, 0) != 0;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (!hasError)
            {
                CSF_AT(char, context, 0x15) = 0;
                CSF_AT(char, context, 0x16) = 0;
                ProcessRecord(record, nullptr, context);
            }
            result = ++index;
            offset += 40;
        }
    }

    char error = 1;
    if (!CSF_AT(char, context, 0))
    {
        void* stream = CSF_AT(void*, context, 0x04);
        result = ((bool(__fastcall*)(void*, void*, char*))
                      CSF_AT(void**, stream, 0)[0x7C / 4])(
            stream, nullptr, context);
        if (result)
            error = 0;
    }
    CSF_AT(char, context, 0) = error;
    return result;
}

HOOK(0x004445B0, ProcessRecordArray, "auto");
