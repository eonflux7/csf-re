#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);
CSF_GLOBAL(float, g_MessageBoxSizeScale, 0x00831430);

CSF_FUNC(unsigned int*, __fastcall, sub_526CF0, 0x00526CF0, (void* self, void* edx, unsigned int* a2));

static bool AudioSoundManager_EntryMatches(void* entry, float key)
{
    return CSF_AT(unsigned int, entry, 0x4) != 0
        && CSF_AT(float, entry, 0x8) == key;
}

static unsigned int __fastcall AudioSoundManager_StopSoundByKey(
    void* self, void* /*edx*/, float key)
{
    if (key == g_MissingMetricValue)
    {
        void* sound = CSF_AT(void*, self, 0x44);
        if (sound)
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, sound, 0)[0x2C / 4])(sound, nullptr);
        return static_cast<unsigned int>(-1);
    }

    if (key == g_MessageBoxSizeScale)
    {
        void* sound = CSF_AT(void*, self, 0x48);
        if (sound)
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, sound, 0)[0x2C / 4])(sound, nullptr);
        return static_cast<unsigned int>(-1);
    }

    const unsigned int count = CSF_AT(unsigned int, self, 0x18);
    unsigned int index = 0;
    bool found = false;

    if (static_cast<std::int32_t>(count) >= 4)
    {
        auto* block = CSF_AT(std::uint8_t*, self, 0x10);
        do
        {
            for (unsigned int offset = 0; offset < 4; ++offset)
            {
                if (AudioSoundManager_EntryMatches(block + 12 * offset, key))
                {
                    index += offset;
                    found = true;
                    break;
                }
            }

            if (found)
                break;

            index += 4;
            block += 48;
        } while (index < count - 3);
    }

    if (!found)
    {
        if (index >= count)
            return static_cast<unsigned int>(-1);

        auto* entry = CSF_AT(std::uint8_t*, self, 0x10) + 12 * index;
        while (!AudioSoundManager_EntryMatches(entry, key))
        {
            ++index;
            entry += 12;
            if (index >= count)
                return static_cast<unsigned int>(-1);
        }
    }

    if (index >= count)
        return static_cast<unsigned int>(-1);

    auto* entry = CSF_AT(std::uint8_t*, self, 0x10) + 12 * index;
    void* sound = CSF_AT(void*, entry, 0x4);
    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, sound, 0)[0x2C / 4])(sound, nullptr);

    auto* soundSlot = reinterpret_cast<unsigned int*>(
        CSF_AT(std::uint8_t*, self, 0x10) + 12 * index + 4);
    sub_526CF0(CSF_AT(void*, self, 0x40), nullptr, soundSlot);

    return index;
}

HOOK(0x005221E0, AudioSoundManager_StopSoundByKey, "auto");
