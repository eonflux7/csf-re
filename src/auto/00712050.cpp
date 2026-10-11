#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());
CSF_FUNC(uint32_t*, __cdecl, Audio_CreateSoundInstance, 0x0070E910, (int a2));
CSF_GLOBAL(float, g_CharacterHeightScale, 0x0082D4BC);

static uint32_t* __fastcall Audio_UpdatePendingSounds(
    void* self, void* /*edx*/, int updateArgument)
{
    alignas(8) uint8_t parameters[0x14];

    for (int index = 0; index < 2; ++index)
    {
        const std::size_t offset = 0x8 + index * 4;
        void* sound = CSF_AT(void*, self, offset);
        if (sound)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, sound, 0)[0x24 / 4])(
                    sound, nullptr, updateArgument);

            sound = CSF_AT(void*, self, offset);
            const bool finished =
                ((bool(__fastcall*)(void*, void*))
                    CSF_AT(void**, sound, 0)[0x14 / 4])(sound, nullptr);
            if (finished)
            {
                sound = CSF_AT(void*, self, offset);
                if (sound)
                {
                    ((void(__fastcall*)(void*, void*, int))
                        CSF_AT(void**, sound, 0)[0])(sound, nullptr, 1);
                    CSF_AT(void*, self, offset) = nullptr;
                }
            }
        }
    }

    if (!CSF_AT(uint8_t, self, 0x1B5))
        return nullptr;

    if (CSF_AT(uint8_t, self, 0x18) &&
        !CSF_AT(void*, self, 0x8))
    {
        CSF_AT(uint32_t, parameters, 0x10) = 0;
        CSF_AT(uint8_t, parameters, 0x10) =
            CSF_AT(uint8_t, self, 0x1B1);
        const int soundSetting = CSF_AT(int, self, 0x10);
        CSF_AT(uint32_t, parameters, 0xC) =
            CSF_AT(uint32_t, self, 0x1A8);
        CSF_AT(uint32_t, parameters, 0x0) =
            CSF_AT(uint32_t, self, 0x4);
        CSF_AT(uint8_t, parameters, 0x8) =
            CSF_AT(uint8_t, self, 0x1B2);
        CSF_AT(int, parameters, 0x4) = 0;
        CSF_AT(uint8_t, parameters, 0x9) = 0;
        CSF_AT(uint8_t, parameters, 0xA) = 0;
        CSF_AT(uint8_t, parameters, 0xB) = 0;

        void* audioState = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
        CSF_AT(uint8_t, parameters, 0xB) =
            ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, audioState, 0)[0x6C / 4])(
                    audioState, nullptr) < 3;

        if (CSF_AT(uint8_t, self, 0x1B4))
        {
            CSF_AT(int, parameters, 0x4) = 2;
            CSF_AT(uint8_t, self, 0x1B4) = 0;
        }

        uint32_t* sound =
            Audio_CreateSoundInstance(reinterpret_cast<int>(parameters));
        CSF_AT(uint32_t*, self, 0x8) = sound;
        if (sound)
        {
            if (CSF_AT(uint8_t, self, 0x1B3))
            {
                ((void(__fastcall*)(void*, void*, int, int))
                    CSF_AT(void**, sound, 0)[0x8 / 4])(
                        sound, nullptr, 1, 0);
            }
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, sound, 0)[0x18 / 4])(
                    sound, nullptr, soundSetting);
            ((uint32_t*(__fastcall*)(void*, void*, uint8_t*, uint32_t,
                                     uint32_t, int))
                CSF_AT(void**, sound, 0)[0x4 / 4])(
                    sound, nullptr,
                    reinterpret_cast<uint8_t*>(self) + 0x18,
                    CSF_AT(uint32_t, parameters, 0xC),
                    CSF_AT(uint32_t, parameters, 0x10), 1);
        }
        CSF_AT(uint8_t, self, 0x18) = 0;
    }

    uint8_t* pendingSound = reinterpret_cast<uint8_t*>(self) + 0xE0;
    uint32_t* result = reinterpret_cast<uint32_t*>(pendingSound);
    if (CSF_AT(uint8_t, self, 0xE0) &&
        !CSF_AT(void*, self, 0xC))
    {
        CSF_AT(uint32_t, parameters, 0x10) =
            CSF_AT(uint32_t, self, 0x1AC);
        CSF_AT(int, parameters, 0xC) = CSF_AT(int, self, 0x14);
        CSF_AT(uint32_t, parameters, 0x0) =
            CSF_AT(uint32_t, self, 0x4);
        CSF_AT(uint8_t, parameters, 0xA) =
            CSF_AT(uint8_t, self, 0x1B0);
        const bool specialStart = CSF_AT(uint8_t, self, 0x1B4) != 0;
        CSF_AT(uint8_t, parameters, 0x8) =
            CSF_AT(uint8_t, self, 0x1B2);
        CSF_AT(int, parameters, 0x4) = 0;
        CSF_AT(uint8_t, parameters, 0x9) = 1;
        CSF_AT(uint8_t, parameters, 0xB) = 0;
        if (specialStart)
        {
            CSF_AT(int, parameters, 0x4) = 2;
            CSF_AT(uint8_t, self, 0x1B4) = 0;
        }

        result = Audio_CreateSoundInstance(reinterpret_cast<int>(parameters));
        uint32_t* sound = result;
        CSF_AT(uint32_t*, self, 0xC) = sound;
        if (sound)
        {
            if (CSF_AT(uint8_t, self, 0x1B3))
            {
                ((void(__fastcall*)(void*, void*, int, int))
                    CSF_AT(void**, sound, 0)[0x8 / 4])(
                        sound, nullptr, 1, 0);
            }
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, sound, 0)[0x18 / 4])(
                    sound, nullptr, CSF_AT(int, parameters, 0xC));

            const float scaledValue =
                CSF_AT(float, parameters, 0x10) * g_CharacterHeightScale;
            uint32_t scaledArgument;
            std::memcpy(&scaledArgument, &scaledValue, sizeof(scaledArgument));
            result =
                ((uint32_t*(__fastcall*)(void*, void*, uint8_t*, uint32_t,
                                         uint32_t, int))
                    CSF_AT(void**, sound, 0)[0x4 / 4])(
                        sound, nullptr, pendingSound, scaledArgument, 0, 0);
        }
        *pendingSound = 0;
    }
    return result;
}

HOOK(0x00712050, Audio_UpdatePendingSounds, "auto");
