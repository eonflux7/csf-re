#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char*, __fastcall, sub_4EF9E0, 0x004EF9E0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_4EFB00, 0x004EFB00, (void* self, void* edx, void* a2));

static char* __stdcall TextDatabase_ReadEntries(void* entries, char* context)
{
    const char savedMode = CSF_AT(char, context, 0x15);
    const bool useExistingCount = CSF_AT(char, context, 0x14) != 0;
    const char initialError = CSF_AT(char, context, 0x00);

    uint32_t entryCount = 0;
    if (useExistingCount)
        entryCount = CSF_AT(uint32_t, entries, 0x08);

    bool beginFailed = false;
    if (!initialError)
    {
        void* reader = CSF_AT(void*, context, 0x04);
        beginFailed =
            !((bool(__fastcall*)(void*, void*, uint32_t*, char*))
                  CSF_AT(void**, reader, 0x00)[0x78 / 4])(
                reader, nullptr, &entryCount, context);
    }

    char* result = reinterpret_cast<char*>(static_cast<uintptr_t>(beginFailed));
    CSF_AT(char, context, 0x00) = static_cast<char>(beginFailed);
    if (beginFailed)
    {
        CSF_AT(char, context, 0x00) = 1;
        return result;
    }

    if (useExistingCount || savedMode)
    {
        uint32_t index = 0;
        uint32_t offset = 0;
        while (index < entryCount)
        {
            void* entry = CSF_AT(uint8_t*, entries, 0x00) + offset;
            const bool canRead = CSF_AT(char, context, 0x00) == 0;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (canRead)
            {
                CSF_AT(char, context, 0x15) = savedMode;
                CSF_AT(char, context, 0x16) = 0;
                sub_4EFB00(entry, nullptr, context);
            }
            ++index;
            result = reinterpret_cast<char*>(static_cast<uintptr_t>(index));
            offset += 32;
        }
    }
    else
    {
        const uint32_t capacity = CSF_AT(uint32_t, entries, 0x04);
        result = reinterpret_cast<char*>(static_cast<uintptr_t>(capacity));
        CSF_AT(uint32_t, entries, 0x08) = 0;
        if (entryCount > capacity)
            result = sub_4EF9E0(
                entries, nullptr, static_cast<int>(entryCount - capacity));

        uint32_t index = 0;
        uint32_t offset = 0;
        while (index < entryCount)
        {
            if (CSF_AT(uint32_t, entries, 0x08) >=
                CSF_AT(uint32_t, entries, 0x04))
            {
                sub_4EF9E0(entries, nullptr, CSF_AT(int, entries, 0x0C));
            }

            const uint32_t usedCount = CSF_AT(uint32_t, entries, 0x08);
            uint8_t* newEntry =
                CSF_AT(uint8_t*, entries, 0x00) + 32 * usedCount;
            CSF_AT(uint32_t, entries, 0x08) = usedCount + 1;

            CSF_AT(int32_t, newEntry, 0x00) = -1;
            CSF_AT(int32_t, newEntry, 0x04) = -1;
            CSF_AT(int32_t, newEntry, 0x0C) = -1;
            CSF_AT(int32_t, newEntry, 0x10) = -1;
            CSF_AT(int32_t, newEntry, 0x14) = -1;
            CSF_AT(uint8_t, newEntry, 0x18) = 0;
            CSF_AT(uint32_t, newEntry, 0x1C) = 0;

            void* entry = CSF_AT(uint8_t*, entries, 0x00) + offset;
            const bool canRead = CSF_AT(char, context, 0x00) == 0;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (canRead)
            {
                CSF_AT(char, context, 0x15) = 0;
                CSF_AT(char, context, 0x16) = 0;
                sub_4EFB00(entry, nullptr, context);
            }

            ++index;
            result = reinterpret_cast<char*>(static_cast<uintptr_t>(index));
            offset += 32;
        }
    }

    char error = 1;
    if (CSF_AT(char, context, 0x00) == 0)
    {
        void* reader = CSF_AT(void*, context, 0x04);
        const int finishResult =
            ((int(__fastcall*)(void*, void*, char*))
                 CSF_AT(void**, reader, 0x00)[0x7C / 4])(
                reader, nullptr, context);
        result = reinterpret_cast<char*>(static_cast<uintptr_t>(finishResult));
        if (static_cast<uint8_t>(finishResult) != 0)
            error = 0;
    }
    CSF_AT(char, context, 0x00) = error;
    return result;
}

HOOK(0x004EFD60, TextDatabase_ReadEntries, "auto");
