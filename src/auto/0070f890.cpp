#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t, __cdecl, CoreFiles_CloseSceneFile, 0x00697980, (void* file));
CSF_FUNC(void*, __cdecl, CoreFiles_OpenSceneFile, 0x00698350, (const char* filename, char* mode, int openFlags));
CSF_FUNC(char, __cdecl, Audio_ReadWaveHeader, 0x00712B70, (int* stream, std::uint32_t* header));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

static double __fastcall C_00863ba8_GetSoundDuration(
    C_00863ba8* self, void* /*edx*/, const char* filename)
{
    if (!filename || !*filename)
        return g_fZero;

    alignas(8) std::uint8_t headerBytes[0x2C];

    auto* sound = reinterpret_cast<C_00863ba8* (__fastcall*)(
        void*, void*, char*, int, int, int)>(
        self->vtbl[0x84 / 4])(
            self, nullptr, const_cast<char*>(filename), 0, 0, 0);

    if (sound)
    {
        std::memcpy(headerBytes + 0x10, &sound->field_002c, 0x1C);

        const std::uint32_t sampleCount =
            (8U * CSF_AT(std::uint32_t, headerBytes, 0x18)) /
            CSF_AT(std::uint8_t, headerBytes, 0x1C);

        return static_cast<double>(sampleCount) /
               static_cast<double>(
                   CSF_AT(std::uint32_t, headerBytes, 0x10));
    }

    float duration = 0.0f;
    void* file = CoreFiles_OpenSceneFile(
        filename, const_cast<char*>(CSF_STR(0x008579F8, "rb")), 0);

    if (file)
    {
        if (Audio_ReadWaveHeader(
                static_cast<int*>(file),
                reinterpret_cast<std::uint32_t*>(headerBytes)))
        {
            const std::uint32_t sampleCount =
                (8U * CSF_AT(std::uint32_t, headerBytes, 0x28)) /
                CSF_AT(std::uint16_t, headerBytes, 0x22) /
                CSF_AT(std::uint16_t, headerBytes, 0x16);

            duration = static_cast<float>(
                static_cast<double>(sampleCount) /
                static_cast<double>(
                    CSF_AT(std::uint32_t, headerBytes, 0x18)));
        }

        CoreFiles_CloseSceneFile(file);
    }

    return duration;
}

HOOK(0x0070F890, C_00863ba8_GetSoundDuration, "auto");
