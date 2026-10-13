#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __stdcall, AudioSoundManager_ProcessArchiveContext, 0x00405450, (void* context));

static int __fastcall AudioSoundManager_ProcessArchiveEntries(
    void* self, void* /*edx*/, void* context)
{
    bool failed = false;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, archive, 0)[0x80 / 4])(
                archive, nullptr, 0, context);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0) = failed;
    AudioSoundManager_ProcessArchiveContext(context);

    const uint32_t entriesBegin = CSF_AT(uint32_t, self, 0x50);
    int32_t entryCount = 0;
    if (entriesBegin != 0)
    {
        entryCount = static_cast<int32_t>(
            CSF_AT(uint32_t, self, 0x54) - entriesBegin) / 44;
    }

    uint32_t entryOffset = 0;
    while (entryCount > 0)
    {
        const uint32_t entryAddress =
            CSF_AT(uint32_t, self, 0x50) + entryOffset;
        const bool alreadyFailed = CSF_AT(uint8_t, context, 0) != 0;
        CSF_AT(uint32_t, context, 0x1C) = 0;

        if (!alreadyFailed)
        {
            void* archive = CSF_AT(void*, context, 0x04);
            void* entryValue = reinterpret_cast<void*>(entryAddress + 0x28);
            CSF_AT(uint8_t, context, 0x15) = 0;
            CSF_AT(uint8_t, context, 0x16) = 0;

            const int result =
                reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                    CSF_AT(void**, archive, 0)[0x58 / 4])(
                        archive, nullptr, entryValue, context);
            CSF_AT(uint8_t, context, 0) = result == -1;
        }

        entryOffset += 44;
        --entryCount;
    }

    CSF_AT(uint32_t, context, 0x1C) = savedContext;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        if (reinterpret_cast<bool(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, archive, 0)[0x7C / 4])(
                    archive, nullptr, context))
        {
            CSF_AT(uint8_t, context, 0) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x00708FC0, AudioSoundManager_ProcessArchiveEntries, "auto");
