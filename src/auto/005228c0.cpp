#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static void __fastcall CSoundManager_SetMasterAndEffectsVolumes(
    CSoundManager* self, void* /*edx*/, float masterVolume, float effectsVolume)
{
    if (self->m_masterVolume == masterVolume &&
        self->m_effectsVolume == effectsVolume)
        return;

    self->m_masterVolume = masterVolume;
    uint32_t remaining = self->field_0018;
    self->m_effectsVolume = effectsVolume;

    uint32_t offset = 0;
    while (remaining)
    {
        void* sound = CSF_AT(
            void*, reinterpret_cast<uint8_t*>(self->field_0010) + offset, 4);
        if (sound)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, float, float)>(
                CSF_AT(void**, sound, 0)[104 / 4])(
                sound, nullptr, self->m_masterVolume, self->m_effectsVolume);
        }
        offset += 12;
        --remaining;
    }

    remaining = self->field_0028;
    offset = 0;
    while (remaining)
    {
        void* sound = CSF_AT(
            void*, reinterpret_cast<uint8_t*>(self->field_0020) + offset, 4);
        if (sound)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, float, float)>(
                CSF_AT(void**, sound, 0)[104 / 4])(
                sound, nullptr, self->m_masterVolume, self->m_effectsVolume);
        }
        offset += 24;
        --remaining;
    }

    remaining = self->field_0038;
    offset = 0;
    while (remaining)
    {
        void* sound = CSF_AT(
            void*, reinterpret_cast<uint8_t*>(self->field_0030) + offset, 4);
        if (sound)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, float, float)>(
                CSF_AT(void**, sound, 0)[104 / 4])(
                sound, nullptr, self->m_masterVolume, self->m_effectsVolume);
        }
        offset += 24;
        --remaining;
    }

    remaining = self->field_0054;
    offset = 0;
    while (remaining)
    {
        void* sound = CSF_AT(
            void*, reinterpret_cast<uint8_t*>(self->m_slots) + offset, 8);
        if (sound)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, float, float)>(
                CSF_AT(void**, sound, 0)[104 / 4])(
                sound, nullptr, self->m_masterVolume, self->m_effectsVolume);
        }
        offset += 12;
        --remaining;
    }

    void* sound = reinterpret_cast<void*>(self->field_0044);
    if (sound)
    {
        if (self->m_masterVolume == 1.0f)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, sound, 0)[48 / 4])(sound, nullptr, 0);

            sound = reinterpret_cast<void*>(self->field_0044);
            reinterpret_cast<void(__fastcall*)(void*, void*, float, float)>(
                CSF_AT(void**, sound, 0)[104 / 4])(
                sound, nullptr, self->m_masterVolume, self->m_effectsVolume);
        }
        else
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, sound, 0)[48 / 4])(sound, nullptr, 1);
        }
    }
}

HOOK(0x005228C0, CSoundManager_SetMasterAndEffectsVolumes, "auto");
