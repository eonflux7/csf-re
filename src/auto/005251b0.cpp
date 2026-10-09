#include "hook.h"
#include "globals.h"
#include "core/memory.h"

static void __fastcall AudioSoundManager_AdvanceInstanceTimers(
    void* self, void* /*edx*/, float elapsedTime)
{
    CSF_AT(float, self, 0x60) = elapsedTime + CSF_AT(float, self, 0x60);
    CSF_AT(float, self, 0x64) = elapsedTime + CSF_AT(float, self, 0x64);
    CSF_AT(float, self, 0x68) = elapsedTime + CSF_AT(float, self, 0x68);
    CSF_AT(float, self, 0x6C) = elapsedTime + CSF_AT(float, self, 0x6C);
    CSF_AT(float, self, 0x70) = elapsedTime + CSF_AT(float, self, 0x70);
    CSF_AT(float, self, 0x74) = elapsedTime + CSF_AT(float, self, 0x74);
    CSF_AT(float, self, 0x78) = elapsedTime + CSF_AT(float, self, 0x78);
    CSF_AT(float, self, 0x7C) = elapsedTime + CSF_AT(float, self, 0x7C);
    CSF_AT(float, self, 0x80) = elapsedTime + CSF_AT(float, self, 0x80);
    CSF_AT(float, self, 0x84) = elapsedTime + CSF_AT(float, self, 0x84);
    CSF_AT(float, self, 0x88) = elapsedTime + CSF_AT(float, self, 0x88);

    void* sound = CSF_AT(void*, self, 0x5C);
    if (sound)
    {
        const bool active =
            reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                CSF_AT(void**, sound, 0)[0x3C / 4])(sound, nullptr);
        if (!active)
        {
            sound = CSF_AT(void*, self, 0x5C);
            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, sound, 0)[0x4C / 4])(sound, nullptr);
            CSF_AT(void*, self, 0x5C) = nullptr;
        }
    }
}

HOOK(0x005251B0, AudioSoundManager_AdvanceInstanceTimers, "auto");
