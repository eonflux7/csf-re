#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, AudioSoundManager_GrowSoundEntries, 0x00522D00,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AudioSoundManager_ProcessSoundEntry, 0x00523EE0,
         (void* self, void* edx, int a2));

static int __stdcall AudioSoundManager_ProcessSoundEntries(int* entries, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    const uint8_t reuseEntries = CSF_AT(uint8_t, context, 0x14);
    const uint8_t savedMode = CSF_AT(uint8_t, context, 0x15);

    uint32_t requestedCount = 0;
    if (reuseEntries)
        requestedCount = CSF_AT(uint32_t, entries, 0x08);

    bool preparationFailed = false;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        preparationFailed =
            !((bool(__fastcall*)(void*, void*, uint32_t*, int))
                  CSF_AT(void**, processor, 0x00)[120 / 4])(
                processor, nullptr, &requestedCount, contextAddress);
    }
    CSF_AT(uint8_t, context, 0x00) = preparationFailed;

    if (!preparationFailed)
    {
        if (reuseEntries || savedMode)
        {
            uint32_t index = 0;
            uint32_t byteOffset = 0;
            while (index < requestedCount)
            {
                void* entry = CSF_AT(uint8_t*, entries, 0x00) + byteOffset;
                const bool canProcess = CSF_AT(uint8_t, context, 0x00) == 0;
                CSF_AT(uint32_t, context, 0x1C) = 0;
                if (canProcess)
                {
                    CSF_AT(uint8_t, context, 0x15) = savedMode;
                    CSF_AT(uint8_t, context, 0x16) = 0;
                    AudioSoundManager_ProcessSoundEntry(entry, nullptr, contextAddress);
                }
                ++index;
                byteOffset += 12;
            }
        }
        else
        {
            const uint32_t capacity = CSF_AT(uint32_t, entries, 0x04);
            CSF_AT(uint32_t, entries, 0x08) = 0;
            if (requestedCount > capacity)
            {
                AudioSoundManager_GrowSoundEntries(
                    entries, nullptr, static_cast<int>(requestedCount - capacity));
            }

            uint32_t index = 0;
            uint32_t byteOffset = 0;
            while (index < requestedCount)
            {
                if (CSF_AT(uint32_t, entries, 0x08) >=
                    CSF_AT(uint32_t, entries, 0x04))
                {
                    AudioSoundManager_GrowSoundEntries(
                        entries, nullptr, CSF_AT(int, entries, 0x0C));
                }

                const uint32_t entryCount = CSF_AT(uint32_t, entries, 0x08);
                void* newEntry = CSF_AT(uint8_t*, entries, 0x00) + 12 * entryCount;
                CSF_AT(uint32_t, entries, 0x08) = entryCount + 1;
                CSF_AT(uint32_t, newEntry, 0x00) = 0;
                CSF_AT(float, newEntry, 0x04) = -1.0f;
                CSF_AT(uint32_t, newEntry, 0x08) = 0;

                uint8_t* entryData = CSF_AT(uint8_t*, entries, 0x00);
                CSF_AT(uint32_t, context, 0x1C) = 0;
                void* entry = entryData + byteOffset;
                if (CSF_AT(uint8_t, context, 0x00) == 0)
                {
                    CSF_AT(uint8_t, context, 0x15) = 0;
                    CSF_AT(uint8_t, context, 0x16) = 0;
                    AudioSoundManager_ProcessSoundEntry(entry, nullptr, contextAddress);
                }
                byteOffset += 12;
                ++index;
            }
        }

        if (CSF_AT(uint8_t, context, 0x00) == 0)
        {
            void* processor = CSF_AT(void*, context, 0x04);
            if (((bool(__fastcall*)(void*, void*, int))
                     CSF_AT(void**, processor, 0x00)[124 / 4])(
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

HOOK(0x00524A00, AudioSoundManager_ProcessSoundEntries, "auto");
