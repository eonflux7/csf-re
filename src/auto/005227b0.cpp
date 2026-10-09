#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static void AudioSoundManager_StopActiveSound(void* sound)
{
    if (sound &&
        reinterpret_cast<bool(__fastcall*)(void*, void*)>(
            CSF_AT(void**, sound, 0)[0x44 / 4])(sound, nullptr))
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, sound, 0)[0x30 / 4])(sound, nullptr, 0);
    }
}

static char __fastcall AudioSoundManager_ProcessPendingStops(void* self, void* /*edx*/)
{
    if (!CSF_AT(std::uint8_t, self, 0x79))
        return 0;

    std::uint32_t remaining = CSF_AT(std::uint32_t, self, 0x18);
    CSF_AT(std::uint8_t, self, 0x79) = 0;

    std::uint32_t offset = 0;
    while (remaining)
    {
        auto* entries = CSF_AT(std::uint8_t*, self, 0x10);
        void* sound = CSF_AT(void*, entries + offset, 0x4);
        AudioSoundManager_StopActiveSound(sound);
        offset += 12;
        --remaining;
    }

    remaining = CSF_AT(std::uint32_t, self, 0x28);
    offset = 0;
    while (remaining)
    {
        auto* entries = CSF_AT(std::uint8_t*, self, 0x20);
        void* sound = CSF_AT(void*, entries + offset, 0x4);
        AudioSoundManager_StopActiveSound(sound);
        offset += 24;
        --remaining;
    }

    remaining = CSF_AT(std::uint32_t, self, 0x38);
    offset = 0;
    while (remaining)
    {
        auto* entries = CSF_AT(std::uint8_t*, self, 0x30);
        void* sound = CSF_AT(void*, entries + offset, 0x4);
        AudioSoundManager_StopActiveSound(sound);
        offset += 24;
        --remaining;
    }

    remaining = CSF_AT(std::uint32_t, self, 0x54);
    offset = 0;
    while (remaining)
    {
        auto* entries = CSF_AT(std::uint8_t*, self, 0x4C);
        void* sound = CSF_AT(void*, entries + offset, 0x8);
        AudioSoundManager_StopActiveSound(sound);
        offset += 12;
        --remaining;
    }

    return 0;
}

HOOK(0x005227B0, AudioSoundManager_ProcessPendingStops, "auto");
