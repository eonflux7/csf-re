#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));

static const auto g_ArrayCapacityVtable = CSF_VTBL(0x0041E5F0);

static unsigned int __stdcall Scene_SerializeEntryList(
    S_0041e5f0* entries, char* context)
{
    const char savedMode = context[21];
    const char initialFlag = *context;
    const bool reuseEntries = context[20] != 0;

    uint32_t entryCount = 0;
    if (reuseEntries)
        entryCount = entries->field_0008;

    unsigned int result = 0;
    if (!initialFlag)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        result = !((bool(__fastcall*)(void*, void*, uint32_t*, char*))
            CSF_AT(void**, serializer, 0)[120 / 4])(
                serializer, nullptr, &entryCount, context);
    }

    *context = static_cast<char>(result);
    if (result)
    {
        *context = 1;
        return result;
    }

    if (reuseEntries)
    {
        result = entryCount;
        for (uint32_t index = 0; index < entryCount; ++index)
        {
            const uint32_t entryAddress = entries->field_0000 + 4 * index;
            const bool processEntry = *context == 0;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (processEntry)
            {
                void* serializer = CSF_AT(void*, context, 0x04);
                context[21] = savedMode;
                context[22] = 0;
                *context = ((int(__fastcall*)(void*, void*, uint32_t, char*))
                    CSF_AT(void**, serializer, 0)[72 / 4])(
                        serializer, nullptr, entryAddress, context) == -1;
            }
            result = entryCount;
        }
    }
    else if (savedMode)
    {
        result = entryCount;
        for (uint32_t index = 0; index < entryCount; ++index)
        {
            const bool processEntry = *context == 0;
            const uint32_t entryAddress = entries->field_0000 + 4 * index;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (processEntry)
            {
                context[21] = savedMode;
                void* serializer = CSF_AT(void*, context, 0x04);
                context[22] = 0;
                *context = ((int(__fastcall*)(void*, void*, uint32_t, char*))
                    CSF_AT(void**, serializer, 0)[72 / 4])(
                        serializer, nullptr, entryAddress, context) == -1;
            }
            result = entryCount;
        }
    }
    else
    {
        result = entries->field_0004;
        uint32_t requestedCount = entryCount;
        entries->field_0008 = 0;
        if (requestedCount > result)
        {
            result = reinterpret_cast<uintptr_t>(
                S_0041e5f0_GrowArrayCapacity(
                    entries, nullptr,
                    static_cast<int>(requestedCount - result)));
            requestedCount = entryCount;
        }

        if (requestedCount)
        {
            uint32_t index = 0;
            do
            {
                if (entries->field_0008 >= entries->field_0004)
                {
                    S_0041e5f0_GrowArrayCapacity(
                        entries, nullptr,
                        static_cast<int>(entries->field_000c));
                }

                const uint32_t size = entries->field_0008;
                uint32_t* newEntry = reinterpret_cast<uint32_t*>(
                    entries->field_0000 + 4 * size);
                entries->field_0008 = size + 1;
                *newEntry = 0;

                const uint32_t entryAddress =
                    entries->field_0000 + 4 * index;
                const bool processEntry = *context == 0;
                CSF_AT(uint32_t, context, 0x1C) = 0;
                if (processEntry)
                {
                    void* serializer = CSF_AT(void*, context, 0x04);
                    context[21] = 0;
                    context[22] = 0;
                    *context = ((int(__fastcall*)(void*, void*, uint32_t, char*))
                        CSF_AT(void**, serializer, 0)[72 / 4])(
                            serializer, nullptr, entryAddress, context) == -1;
                }

                result = ++index;
            }
            while (index < entryCount);
        }
    }

    char failed = 0;
    if (*context)
    {
        failed = 1;
    }
    else
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        result = ((int(__fastcall*)(void*, void*, char*))
            CSF_AT(void**, serializer, 0)[124 / 4])(
                serializer, nullptr, context);
        if (static_cast<uint8_t>(result) == 0)
            failed = 1;
    }

    *context = failed;
    return result;
}

HOOK(0x0044D680, Scene_SerializeEntryList, "auto");
