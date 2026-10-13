#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, SpawnPoints_ProcessEntryPair, 0x0048D240,
         (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, SpawnPoints_GrowEntryBuffer, 0x00551CF0, (void* self, void* , int extraEntries));

static int __stdcall SpawnPoints_ProcessEntryPairs(unsigned int entriesAddress,
                                                   int contextAddress)
{
    void* entries = reinterpret_cast<void*>(entriesAddress);
    void* context = reinterpret_cast<void*>(contextAddress);
    const uint8_t savedMode = CSF_AT(uint8_t, context, 0x15);
    const uint8_t initialState = CSF_AT(uint8_t, context, 0x00);
    const bool useExistingCount = CSF_AT(uint8_t, context, 0x14) != 0;

    uint32_t count = 0;
    if (useExistingCount)
        count = CSF_AT(uint32_t, entries, 0x08);

    bool failed = false;
    if (!initialState)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, uint32_t*, int)>(
            CSF_AT(void**, processor, 0x00)[0x78 / 4])(
                processor, nullptr, &count, contextAddress);
    }
    CSF_AT(uint8_t, context, 0x00) = failed;

    if (!failed)
    {
        if (useExistingCount || savedMode)
        {
            for (uint32_t index = 0; index < count; ++index)
            {
                const bool canProcess = CSF_AT(uint8_t, context, 0x00) == 0;
                void* entry = static_cast<uint8_t*>(
                    CSF_AT(void*, entries, 0x00)) + 8 * index;
                CSF_AT(uint32_t, context, 0x1C) = 0;
                if (canProcess)
                {
                    CSF_AT(uint8_t, context, 0x15) = savedMode;
                    CSF_AT(uint8_t, context, 0x16) = 0;
                    SpawnPoints_ProcessEntryPair(entry, nullptr, contextAddress);
                }
            }
        }
        else
        {
            const uint32_t capacity = CSF_AT(uint32_t, entries, 0x04);
            CSF_AT(uint32_t, entries, 0x08) = 0;
            if (count > capacity)
            {
                SpawnPoints_GrowEntryBuffer(
                    entries, nullptr, static_cast<int>(count - capacity));
            }

            for (uint32_t index = 0; index < count; ++index)
            {
                if (CSF_AT(uint32_t, entries, 0x08) >=
                    CSF_AT(uint32_t, entries, 0x04))
                {
                    SpawnPoints_GrowEntryBuffer(
                        entries, nullptr, CSF_AT(int, entries, 0x0C));
                }

                const uint32_t size = CSF_AT(uint32_t, entries, 0x08);
                void* newEntry = static_cast<uint8_t*>(
                    CSF_AT(void*, entries, 0x00)) + 8 * size;
                CSF_AT(uint32_t, entries, 0x08) = size + 1;
                CSF_AT(uint32_t, newEntry, 0x04) = 0;
                CSF_AT(uint32_t, newEntry, 0x00) = 0;

                void* entry = static_cast<uint8_t*>(
                    CSF_AT(void*, entries, 0x00)) + 8 * index;
                const bool canProcess = CSF_AT(uint8_t, context, 0x00) == 0;
                CSF_AT(uint32_t, context, 0x1C) = 0;
                if (canProcess)
                {
                    CSF_AT(uint8_t, context, 0x15) = 0;
                    CSF_AT(uint8_t, context, 0x16) = 0;
                    SpawnPoints_ProcessEntryPair(entry, nullptr, contextAddress);
                }
            }
        }

        if (!CSF_AT(uint8_t, context, 0x00))
        {
            void* processor = CSF_AT(void*, context, 0x04);
            if (reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, processor, 0x00)[0x7C / 4])(
                    processor, nullptr, contextAddress))
            {
                CSF_AT(uint8_t, context, 0x00) = 0;
                return 0;
            }
        }
    }

    CSF_AT(uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x0048D300, SpawnPoints_ProcessEntryPairs, "auto");
