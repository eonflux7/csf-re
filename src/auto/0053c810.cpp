#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, GetSoundConfiguration, 0x0075FDF0, ());
CSF_FUNC(int, __cdecl, GetAcousticCapabilities, 0x0070DDE0, ());

static int __fastcall CMenuSoundOptions_InitializeSoundSwitch(
    CMenuSoundOptions* self, void* /*edx*/, void* controls)
{
    if (!self->field_002c)
        return 0;

    void* soundSwitch =
        ((void*(__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, controls, 0)[0xFC / 4])(
                controls, nullptr, CSF_STR(0x00842A64, "Switch"));

    self->field_0024 = reinterpret_cast<std::uint32_t>(soundSwitch);
    if (!soundSwitch)
        return 0;

    void* configuration = reinterpret_cast<void*>(GetSoundConfiguration());
    const int enabled = CSF_AT(std::uint8_t, configuration, 0xCC);
    soundSwitch = reinterpret_cast<void*>(self->field_0024);
    ((int(__fastcall*)(void*, void*, int, int, int))
        CSF_AT(void**, soundSwitch, 0)[0x68 / 4])(
            soundSwitch, nullptr, enabled, 1, 1);

    void* capabilities = reinterpret_cast<void*>(GetAcousticCapabilities());
    const unsigned char supported =
        ((unsigned char(__fastcall*)(void*, void*))
            CSF_AT(void**, capabilities, 0)[0x50 / 4])(
                capabilities, nullptr);

    soundSwitch = reinterpret_cast<void*>(self->field_0024);
    void** switchVtable = CSF_AT(void**, soundSwitch, 0);
    if (!supported)
    {
        ((void(__fastcall*)(void*, void*, int))
            switchVtable[0x29C / 4])(soundSwitch, nullptr, 0x534F3131);

        soundSwitch = reinterpret_cast<void*>(self->field_0024);
        return ((int(__fastcall*)(void*, void*, int, int, int))
            CSF_AT(void**, soundSwitch, 0)[0x68 / 4])(
                soundSwitch, nullptr, 0, 1, 1);
    }

    ((void(__fastcall*)(void*, void*, int))
        switchVtable[0x29C / 4])(soundSwitch, nullptr, 0x534F3130);

    soundSwitch = reinterpret_cast<void*>(self->field_0024);
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, soundSwitch, 0)[0x29C / 4])(
            soundSwitch, nullptr, 0x534F3131);

    void* settings = reinterpret_cast<void*>(self->field_002c);
    soundSwitch = reinterpret_cast<void*>(self->field_0024);
    void** settingsVtable = CSF_AT(void**, settings, 0);
    switchVtable = CSF_AT(void**, soundSwitch, 0);
    const unsigned char active =
        ((unsigned char(__fastcall*)(void*, void*))
            settingsVtable[0x30 / 4])(settings, nullptr);

    soundSwitch = reinterpret_cast<void*>(self->field_0024);
    return ((int(__fastcall*)(void*, void*, int))
        switchVtable[0x2A4 / 4])(
            soundSwitch, nullptr, 0x534F3131 - (active != 0));
}

HOOK(0x0053C810, CMenuSoundOptions_InitializeSoundSwitch, "auto");
