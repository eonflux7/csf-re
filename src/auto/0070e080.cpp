#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char*, __cdecl, _strstr, 0x007B2350, (char* Str, char* SubStr));

// CRT strrchr at 0x7B2E90: plain __cdecl (caller cleans 8 bytes, callee pops 0).
using CrtStrRChrFn = char* (__cdecl*)(char* Str, int Ch);

CSF_GLOBAL(std::uint32_t, g_StereoPrefixFirstWord, 0x00863994);
CSF_GLOBAL(std::uint32_t, g_StereoPrefixLastWord, 0x00863998);

static char* __stdcall Audio_InsertStereoDirectory(char* path)
{
    char* match = _strstr(
        path, const_cast<char*>(CSF_STR(0x008639A4, "GERMANS")));
    if (match)
        return match;

    match = _strstr(
        path, const_cast<char*>(CSF_STR(0x0086399C, "SCENES")));
    if (match)
        return match;

    const CrtStrRChrFn crtStrRChr =
        reinterpret_cast<CrtStrRChrFn>(CSF_ADDR_FN(0x007B2E90));
    char* separator = crtStrRChr(path, 92);
    if (!separator)
        return separator;

    alignas(8) char pathBuffer[0xFC];
    std::memset(pathBuffer, 0, 248);
    std::memset(pathBuffer + 248, 0, sizeof(std::int16_t));
    std::memcpy(pathBuffer, path, separator - path + 1);

    char* insertion = pathBuffer;
    while (*insertion)
        ++insertion;

    // "STEREO\\" (8 bytes including the terminator) stored as two dwords.
    const std::uint32_t firstWord = g_StereoPrefixFirstWord;
    const std::uint32_t lastWord = g_StereoPrefixLastWord;
    std::memcpy(insertion, &firstWord, sizeof(firstWord));
    std::memcpy(insertion + 4, &lastWord, sizeof(lastWord));

    const char* filename = separator + 1;
    const char* filenameEnd = filename;
    while (*filenameEnd++)
        ;

    insertion = pathBuffer;
    while (*insertion)
        ++insertion;
    std::memcpy(insertion, filename, filenameEnd - filename);

    char* cursor = pathBuffer;
    char* destination = path;
    char character;
    do
    {
        character = *cursor++;
        *destination++ = character;
    } while (character);

    return cursor;
}

HOOK(0x0070E080, Audio_InsertStereoDirectory, "auto");
