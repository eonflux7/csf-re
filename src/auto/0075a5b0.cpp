#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

static int __fastcall C_0086c1e0_FindSoundByWaveName(
    C_0086c1e0* self, void* /*edx*/, char* waveName)
{
    auto* table = reinterpret_cast<S_0075b5e0*>(self->field_0004);
    const uint32_t begin = table->field_0004;
    const uint32_t soundCount = begin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(table->field_0008 - begin) / 68)
        : 0;

    uint32_t soundOffset = 0;
    for (uint32_t soundIndex = 0; soundIndex < soundCount; ++soundIndex)
    {
        table = reinterpret_cast<S_0075b5e0*>(self->field_0004);
        auto* sound = reinterpret_cast<uint8_t*>(table->field_0004 + soundOffset);
        const uint32_t waveBegin = CSF_AT(uint32_t, sound, 0x08);
        uint32_t waveCount = 0;
        if (waveBegin)
        {
            const int32_t byteCount = static_cast<int32_t>(
                CSF_AT(uint32_t, sound, 0x0C) - waveBegin);

            // Signed reciprocal division by the 28-byte wave record size.
            const int64_t product = static_cast<int64_t>(byteCount)
                * static_cast<int32_t>(0x92492493u);
            const int32_t quotient = static_cast<int32_t>(
                ((product >> 32) + byteCount) >> 4);
            waveCount = static_cast<uint32_t>(quotient)
                + (static_cast<uint32_t>(quotient) >> 31);
        }

        uint32_t waveOffset = 0;
        for (uint32_t waveIndex = 0; waveIndex < waveCount; ++waveIndex)
        {
            table = reinterpret_cast<S_0075b5e0*>(self->field_0004);
            sound = reinterpret_cast<uint8_t*>(table->field_0004 + soundOffset);
            auto* wave = reinterpret_cast<uint8_t*>(
                CSF_AT(uint32_t, sound, 0x08) + waveOffset);

            const char* storedName = CSF_AT(uint32_t, wave, 0x18) < 0x10
                ? reinterpret_cast<const char*>(wave + 4)
                : CSF_AT(const char*, wave, 0x04);

            if (storedName && waveName && __stricmp(storedName, waveName) == 0)
                return static_cast<int32_t>(soundIndex);

            waveOffset += 28;
        }

        soundOffset += 68;
    }

    return -1;
}

HOOK(0x0075A5B0, C_0086c1e0_FindSoundByWaveName, "auto");
