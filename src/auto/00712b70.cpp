#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, _strncmp, 0x007B00B0, (const char* name, const char* prefix, std::size_t length));

static char __cdecl Audio_ReadWaveHeader(int* stream, std::uint32_t* header)
{
    alignas(8) std::uint8_t riffHeader[12] = {};
    alignas(8) std::uint8_t formatData[16] = {};
    alignas(8) std::uint8_t chunkHeader[8] = {};

    ((int(__fastcall*)(void*, void*, void*, int))
        CSF_AT(void**, stream, 0)[8 / 4])(stream, nullptr, riffHeader, 12);

    if (_strncmp(reinterpret_cast<const char*>(riffHeader),
                 CSF_STR(0x00863E50, "RIFF"), 4) ||
        _strncmp(reinterpret_cast<const char*>(riffHeader + 8),
                 CSF_STR(0x00863E48, "WAVE"), 4))
        return 0;

    const std::uint32_t riffTag = CSF_AT(std::uint32_t, riffHeader, 0);
    const std::uint32_t riffSize = CSF_AT(std::uint32_t, riffHeader, 4);
    const std::uint32_t waveTag = CSF_AT(std::uint32_t, riffHeader, 8);
    header[0] = riffTag;
    header[1] = riffSize;
    header[2] = waveTag;

    bool foundFormat = false;
    while (static_cast<std::uint32_t>(
               ((int(__fastcall*)(void*, void*, void*, int))
                    CSF_AT(void**, stream, 0)[8 / 4])
                   (stream, nullptr, chunkHeader, 8)) >= 8)
    {
        if (!_strncmp(reinterpret_cast<const char*>(chunkHeader),
                      CSF_STR(0x00863E40, "fmt "), 4))
        {
            ((int(__fastcall*)(void*, void*, void*, int))
                CSF_AT(void**, stream, 0)[8 / 4])
                (stream, nullptr, formatData, 16);

            const std::uint32_t formatSize =
                CSF_AT(std::uint32_t, chunkHeader, 4);
            header[3] = CSF_AT(std::uint32_t, chunkHeader, 0);
            header[5] = CSF_AT(std::uint32_t, formatData, 0);
            header[6] = CSF_AT(std::uint32_t, formatData, 4);
            header[7] = CSF_AT(std::uint32_t, formatData, 8);
            const std::uint32_t finalFormatWord =
                CSF_AT(std::uint32_t, formatData, 12);
            foundFormat = true;
            header[4] = formatSize;
            header[8] = finalFormatWord;

            if (formatSize > 0x10)
            {
                ((void(__fastcall*)(void*, void*, int, std::uint32_t))
                    CSF_AT(void**, stream, 0)[20 / 4])
                    (stream, nullptr, 1, formatSize - 16);
            }
            break;
        }

        ((void(__fastcall*)(void*, void*, int, std::uint32_t))
            CSF_AT(void**, stream, 0)[20 / 4])
            (stream, nullptr, 1, CSF_AT(std::uint32_t, chunkHeader, 4));
    }

    if (!foundFormat)
        return 0;

    while (static_cast<std::uint32_t>(
               ((int(__fastcall*)(void*, void*, void*, int))
                    CSF_AT(void**, stream, 0)[8 / 4])
                   (stream, nullptr, chunkHeader, 8)) >= 8)
    {
        if (!_strncmp(reinterpret_cast<const char*>(chunkHeader),
                      CSF_STR(0x00863E38, "data"), 4))
        {
            const std::uint32_t dataTag =
                CSF_AT(std::uint32_t, chunkHeader, 0);
            const std::uint32_t dataSize =
                CSF_AT(std::uint32_t, chunkHeader, 4);
            header[9] = dataTag;
            header[10] = dataSize;
            return 1;
        }

        ((void(__fastcall*)(void*, void*, int, std::uint32_t))
            CSF_AT(void**, stream, 0)[20 / 4])
            (stream, nullptr, 1, CSF_AT(std::uint32_t, chunkHeader, 4));
    }

    return 0;
}

HOOK(0x00712B70, Audio_ReadWaveHeader, "auto");
