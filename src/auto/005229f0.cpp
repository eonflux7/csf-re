#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, SoundInstance_SetSound, 0x005269B0,
         (void* self, void* edx, int a2));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60,
         (void* destination, const void* source, std::size_t byteCount));

static char* __fastcall AudioSoundManager_RemoveSoundReference(
    void* self, void* /*edx*/, int sound)
{
    const std::uint32_t firstCount = CSF_AT(std::uint32_t, self, 0x10);
    if (firstCount != 0)
    {
        auto* entries = CSF_AT(std::uint8_t*, self, 0x08);
        for (std::uint32_t index = 0; index < firstCount; ++index)
        {
            if (CSF_AT(int, entries + 12 * index, 0x04) != sound)
                continue;

            void* instance = CSF_AT(void*, entries + 12 * index, 0x04);
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, instance, 0)[0x2C / 4])(instance, nullptr);

            const std::uint32_t remaining =
                CSF_AT(std::uint32_t, self, 0x10) - 1;
            CSF_AT(std::uint32_t, self, 0x10) = remaining;
            if (index == remaining)
                return reinterpret_cast<char*>(remaining);

            auto* destination =
                CSF_AT(std::uint8_t*, self, 0x08) + 12 * index;
            return static_cast<char*>(
                _memcpy(destination, destination + 12, 12 * (remaining - index)));
        }
    }

    const std::uint32_t secondCount = CSF_AT(std::uint32_t, self, 0x20);
    if (secondCount != 0)
    {
        auto* entries = CSF_AT(std::uint8_t*, self, 0x18);
        for (std::uint32_t index = 0; index < secondCount; ++index)
        {
            if (CSF_AT(int, entries + 24 * index, 0x04) != sound)
                continue;

            void* instance = CSF_AT(void*, entries + 24 * index, 0x04);
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, instance, 0)[0x2C / 4])(instance, nullptr);

            const std::uint32_t remaining =
                CSF_AT(std::uint32_t, self, 0x20) - 1;
            CSF_AT(std::uint32_t, self, 0x20) = remaining;
            if (index == remaining)
                return reinterpret_cast<char*>(remaining);

            auto* destination =
                CSF_AT(std::uint8_t*, self, 0x18) + 24 * index;
            return static_cast<char*>(
                _memcpy(destination, destination + 24, 24 * (remaining - index)));
        }
    }

    const std::uint32_t thirdCount = CSF_AT(std::uint32_t, self, 0x30);
    if (thirdCount != 0)
    {
        auto* entries = CSF_AT(std::uint8_t*, self, 0x28);
        for (std::uint32_t index = 0; index < thirdCount; ++index)
        {
            if (CSF_AT(int, entries + 24 * index, 0x04) != sound)
                continue;

            void* instance = CSF_AT(void*, entries + 24 * index, 0x04);
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, instance, 0)[0x2C / 4])(instance, nullptr);

            const std::uint32_t remaining =
                CSF_AT(std::uint32_t, self, 0x30) - 1;
            CSF_AT(std::uint32_t, self, 0x30) = remaining;
            if (index == remaining)
                return reinterpret_cast<char*>(remaining);

            auto* destination =
                CSF_AT(std::uint8_t*, self, 0x28) + 24 * index;
            return static_cast<char*>(
                _memcpy(destination, destination + 24, 24 * (remaining - index)));
        }
    }

    const std::uint32_t instanceCount = CSF_AT(std::uint32_t, self, 0x4C);
    if (instanceCount != 0)
    {
        auto* entries = CSF_AT(std::uint8_t*, self, 0x44);
        for (std::uint32_t index = 0; index < instanceCount; ++index)
        {
            void* instance = CSF_AT(void*, entries + 12 * index, 0x08);
            if (CSF_AT(int, instance, 0x60) == sound)
            {
                SoundInstance_SetSound(
                    CSF_AT(void*, entries + 12 * index, 0x08), nullptr, 0);
                return nullptr;
            }
        }
    }

    return reinterpret_cast<char*>(instanceCount);
}

HOOK(0x005229F0, AudioSoundManager_RemoveSoundReference, "auto");
