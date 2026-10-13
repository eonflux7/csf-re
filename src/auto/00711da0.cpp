#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, C_00863d80_CreateSound, 0x00711C10,
         (void* self, void* edx, int a2, float a3, int a4, int a5,
          int a6, int a7, char a8));

static void __fastcall C_00863d80_StartOrUpdateSound(
    C_00863d80* self, void* /*edx*/, char* name, int settingsAddress)
{
    if (!name || !*name)
        return;

    void* settings = reinterpret_cast<void*>(settingsAddress);
    uint32_t* sounds = &self->field_0008;
    int channel = CSF_AT(int, settings, 0x4);

    if (channel)
    {
        if (sounds[channel])
        {
            char* source = name;
            uint8_t* destination = &self->field_00e0;
            char character;
            do
            {
                character = *source++;
                *destination++ = static_cast<uint8_t>(character);
            } while (character);

            self->field_01ac = CSF_AT(uint32_t, settings, 0x0);
            self->field_01b0 = CSF_AT(uint8_t, settings, 0x9);
        }
    }
    else if (self->field_0008)
    {
        void* sound = reinterpret_cast<void*>(
            sounds[CSF_AT(int, settings, 0x4)]);
        bool finished =
            reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                CSF_AT(void**, sound, 0x0)[0xC / 4])(sound, nullptr);

        if (!finished)
        {
            sound = reinterpret_cast<void*>(
                sounds[CSF_AT(int, settings, 0x4)]);
            finished =
                reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, sound, 0x0)[0x14 / 4])(sound, nullptr);
        }

        if (finished)
        {
            sound = reinterpret_cast<void*>(
                sounds[CSF_AT(int, settings, 0x4)]);
            if (sound)
            {
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, sound, 0x0)[0])(sound, nullptr, 1);
                sounds[CSF_AT(int, settings, 0x4)] = 0;
            }
        }
        else
        {
            sound = reinterpret_cast<void*>(
                sounds[CSF_AT(int, settings, 0x4)]);
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, sound, 0x0)[0x10 / 4])(sound, nullptr, 1);

            char* source = name;
            uint8_t* destination = &self->field_0018;
            char character;
            do
            {
                character = *source++;
                *destination++ = static_cast<uint8_t>(character);
            } while (character);

            self->field_01a8 = CSF_AT(uint32_t, settings, 0x0);
            self->field_01b1 = CSF_AT(uint8_t, settings, 0x8);
        }
    }

    channel = CSF_AT(int, settings, 0x4);
    if (sounds[channel])
        return;

    if (self->field_01b5)
    {
        int volumeBits;
        std::memcpy(&volumeBits,
                    channel ? &self->field_0014 : &self->field_0010,
                    sizeof(volumeBits));

        // The original replaces only the low byte of its settings argument.
        int primaryArgument = static_cast<int>(
            (static_cast<uint32_t>(settingsAddress) & 0xFFFFFF00u) |
            static_cast<uint32_t>(channel == 0));
        char secondaryFlag = static_cast<char>(
            CSF_AT(uint8_t, settings, 0x9));
        int primaryFlag = CSF_AT(uint8_t, settings, 0x8);
        float value = CSF_AT(float, settings, 0x0);

        C_00863d80_CreateSound(
            self, nullptr, reinterpret_cast<int>(name), value, volumeBits,
            channel, primaryFlag, primaryArgument, secondaryFlag);
    }
    else if (!channel)
    {
        char* source = name;
        uint8_t* destination = &self->field_0018;
        char character;
        do
        {
            character = *source++;
            *destination++ = static_cast<uint8_t>(character);
        } while (character);

        self->field_01a8 = CSF_AT(uint32_t, settings, 0x0);
        self->field_01b1 = CSF_AT(uint8_t, settings, 0x8);
    }
}

HOOK(0x00711DA0, C_00863d80_StartOrUpdateSound, "auto");
