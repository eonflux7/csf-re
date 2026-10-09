#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(void*, __fastcall, AudioSoundManager_GetSoundFilename, 0x0051BA50, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, CSoundInstance_UpdateSpecialAttachment, 0x005268C0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());
CSF_FUNC(int, __fastcall, AudioSoundManager_GetSoundDescriptor, 0x0075A480, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
// The original uses this constant for its unsigned-to-floating conversion.
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);
CSF_GLOBAL(float, g_NonSpatialVolumeScale, 0x0084119C);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall CSoundInstance_Play(CSoundInstance* self, void* /*edx*/)
{
    void* voice = reinterpret_cast<void*>(self->m_voice);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, voice, 0)[8 / 4])(voice, nullptr);

    self->m_playing = 0;
    self->m_followEntity = 0;
    self->field_005e = 0;
    self->m_loop = 0;
    self->m_fadeTime = 0.0f;
    self->field_004c = 0.0f;
    self->field_0054 = -1;
    self->field_0058 = -1;
    self->field_0064 = 0.0f;
    self->field_0068 = 0.0f;
    self->m_state = 0;
    self->field_005c = 0;

    void* manager = CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FD0);
    const int soundIndex = reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t)>(
        CSF_AT(void**, manager, 0)[0xC / 4])(manager, nullptr, self->m_soundId);

    if (soundIndex != -1)
    {
        void* descriptor = reinterpret_cast<void*>(
            AudioSoundManager_GetSoundDescriptor(manager, nullptr, soundIndex));
        const char spatial = CSF_AT(char, descriptor, 0x21);

        self->m_volume = static_cast<float>(CSF_AT(uint32_t, descriptor, 0x14))
                       * g_ProgressScale;
        if (!spatial)
        {
            void* device = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
            const int quality = reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, device, 0)[0x6C / 4])(device, nullptr);
            if (quality >= 3)
                self->m_volume = self->m_volume * g_NonSpatialVolumeScale;
        }

        const uint32_t variant = self->m_variant;
        uint32_t variantCount = 0;
        if (variant != UINT32_MAX)
        {
            const uint32_t begin = CSF_AT(uint32_t, descriptor, 0x8);
            if (begin)
            {
                variantCount = static_cast<uint32_t>(
                    static_cast<int32_t>(CSF_AT(uint32_t, descriptor, 0xC) - begin) / 28);
            }
        }

        void* filenameObject;
        if (variant == UINT32_MAX || variant >= variantCount)
        {
            filenameObject = AudioSoundManager_GetSoundFilename(
                manager, nullptr, static_cast<unsigned int>(soundIndex));
        }
        else
        {
            filenameObject = reinterpret_cast<void*>(
                CSF_AT(uint32_t, descriptor, 0x8) + 28 * variant);
        }

        char* filename;
        if (CSF_AT(uint32_t, filenameObject, 0x18) < 0x10u)
            filename = reinterpret_cast<char*>(filenameObject) + 4;
        else
            filename = CSF_AT(char*, filenameObject, 0x4);

        void* device = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
        const unsigned int voiceId =
            reinterpret_cast<unsigned int (__fastcall*)(void*, void*, char*, char, int, int)>(
                CSF_AT(void**, device, 0)[0x84 / 4])(
                    device, nullptr, filename, spatial, 0, 0);

        voice = reinterpret_cast<void*>(self->m_voice);
        reinterpret_cast<void (__fastcall*)(void*, void*, unsigned int, char*, char)>(
            CSF_AT(void**, voice, 0)[0x4 / 4])(
                voice, nullptr, voiceId, filename, spatial);

        voice = reinterpret_cast<void*>(self->m_voice);
        reinterpret_cast<void (__fastcall*)(void*, void*, float)>(
            CSF_AT(void**, voice, 0)[0x54 / 4])(voice, nullptr, self->m_volume);

        voice = reinterpret_cast<void*>(self->m_voice);
        const double currentDistance =
            reinterpret_cast<double (__fastcall*)(void*, void*)>(
                CSF_AT(void**, voice, 0)[0x40 / 4])(voice, nullptr);
        if (currentDistance > CSF_AT(float, descriptor, 0x18))
        {
            voice = reinterpret_cast<void*>(self->m_voice);
            reinterpret_cast<double (__fastcall*)(void*, void*)>(
                CSF_AT(void**, voice, 0)[0x40 / 4])(voice, nullptr);
        }

        voice = reinterpret_cast<void*>(self->m_voice);
        reinterpret_cast<void (__fastcall*)(void*, void*, float)>(
            CSF_AT(void**, voice, 0)[0x4C / 4])(
                voice, nullptr, CSF_AT(float, descriptor, 0x18));

        switch (self->m_attachMode)
        {
        case 0:
            self->field_005e = 1;
            break;

        case 1:
            voice = reinterpret_cast<void*>(self->m_voice);
            reinterpret_cast<int (__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, voice, 0)[0x1C / 4])(
                    voice, nullptr, &self->field_001c);
            break;

        case 2:
            self->m_followEntity = 1;
            self->field_0054 = self->field_002c;
            self->field_0058 = self->field_0030;
            if ((self->field_0058 & 0x80000000) == 0)
            {
                const int entity = CHandleTable_Resolve(
                    g_pCurrentScene, nullptr,
                    reinterpret_cast<int>(&self->m_entityHandle));
                if (entity)
                {
                    voice = reinterpret_cast<void*>(self->m_voice);
                    void** voiceVtable = CSF_AT(void**, voice, 0);
                    const int position = GetObjectPositionVector(
                        reinterpret_cast<void*>(entity), nullptr);
                    voice = reinterpret_cast<void*>(self->m_voice);
                    reinterpret_cast<int (__fastcall*)(void*, void*, void*)>(
                        voiceVtable[0x1C / 4])(
                            voice, nullptr, reinterpret_cast<void*>(position));
                }
            }
            break;

        case 4:
            CSoundInstance_UpdateSpecialAttachment(self, nullptr);
            break;

        default:
            break;
        }

        const float durationFraction = self->field_0018;
        const uint8_t loop = self->field_0038;
        self->m_loop = loop;

        if (durationFraction > g_fZero)
        {
            voice = reinterpret_cast<void*>(self->m_voice);
            reinterpret_cast<void (__fastcall*)(void*, void*, bool)>(
                CSF_AT(void**, voice, 0)[0x14 / 4])(voice, nullptr, loop == 0);

            voice = reinterpret_cast<void*>(self->m_voice);
            const double duration =
                reinterpret_cast<double (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, voice, 0)[0x38 / 4])(voice, nullptr);
            self->m_fadeTime = static_cast<float>(duration * self->field_0018);
        }
        else if (self->m_fadeInTime > g_fZero)
        {
            voice = reinterpret_cast<void*>(self->m_voice);
            reinterpret_cast<void (__fastcall*)(void*, void*, bool)>(
                CSF_AT(void**, voice, 0)[0x14 / 4])(voice, nullptr, loop == 0);
            self->m_fadeTime = self->m_fadeInTime;
        }
        else
        {
            voice = reinterpret_cast<void*>(self->m_voice);
            const uint8_t playFlag = loop ? 0 : CSF_AT(uint8_t, descriptor, 0x20);
            reinterpret_cast<void (__fastcall*)(void*, void*, uint8_t)>(
                CSF_AT(void**, voice, 0)[0x14 / 4])(voice, nullptr, playFlag);

            double fadeTime;
            if (self->m_loop || CSF_AT(uint8_t, descriptor, 0x20))
            {
                fadeTime = 0.0;
            }
            else
            {
                voice = reinterpret_cast<void*>(self->m_voice);
                fadeTime = reinterpret_cast<double (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, voice, 0)[0x38 / 4])(voice, nullptr);
            }
            self->m_fadeTime = static_cast<float>(fadeTime);
        }

        self->m_playing = 1;
    }

    return self->m_voice != 0;
}

HOOK(0x00527630, CSoundInstance_Play, "auto");
