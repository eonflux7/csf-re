#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall AudioSoundManager_ResetSoundResources(void* self, void* /*edx*/)
{
    void* sound = CSF_AT(void*, self, 0x08);
    CSF_AT(int32_t, self, 0x00) = -1;
    CSF_AT(int32_t, self, 0x04) = -1;

    int result = 0;
    if (sound)
        result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, sound, 0)[0x2C / 4])(sound, nullptr);

    sound = CSF_AT(void*, self, 0x0C);
    if (sound)
        result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, sound, 0)[0x2C / 4])(sound, nullptr);

    sound = CSF_AT(void*, self, 0x10);
    if (sound)
        result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, sound, 0)[0x2C / 4])(sound, nullptr);

    sound = CSF_AT(void*, self, 0x14);
    if (sound)
        result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, sound, 0)[0x2C / 4])(sound, nullptr);

    sound = CSF_AT(void*, self, 0x18);
    if (sound)
        result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, sound, 0)[0x2C / 4])(sound, nullptr);

    sound = CSF_AT(void*, self, 0x1C);
    if (sound)
        result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, sound, 0)[0x2C / 4])(sound, nullptr);

    sound = CSF_AT(void*, self, 0x20);
    if (sound)
        result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, sound, 0)[0x2C / 4])(sound, nullptr);

    sound = CSF_AT(void*, self, 0x24);
    if (sound)
        result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, sound, 0)[0x2C / 4])(sound, nullptr);

    sound = CSF_AT(void*, self, 0x28);
    if (sound)
        result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, sound, 0)[0x2C / 4])(sound, nullptr);

    sound = CSF_AT(void*, self, 0x2C);
    if (sound)
        result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, sound, 0)[0x2C / 4])(sound, nullptr);

    sound = CSF_AT(void*, self, 0x30);
    if (sound)
        result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, sound, 0)[0x2C / 4])(sound, nullptr);

    void* resource = CSF_AT(void*, self, 0x34);
    if (resource)
    {
        result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, resource, 0)[0x4C / 4])(resource, nullptr);
        CSF_AT(void*, self, 0x34) = nullptr;
    }

    return result;
}

HOOK(0x00524FB0, AudioSoundManager_ResetSoundResources, "auto");
