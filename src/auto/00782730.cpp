#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));
CSF_GLOBAL(const char*, g_psz_DEFAULT, 0x008A9BF4);

static char __fastcall LevelRegistry_SetLevelName(
    void* self, void* /*edx*/, char* levelName)
{
    alignas(8) uint8_t nameBytes[0x104];
    std::memset(nameBytes, 0, 253);
    nameBytes[253] = 0;
    nameBytes[254] = 0;
    nameBytes[255] = 0;

    const char* source =
        levelName && *levelName ? levelName : g_psz_DEFAULT;
    char* destination = reinterpret_cast<char*>(nameBytes);
    char character;
    do
    {
        character = *source++;
        *destination++ = character;
    } while (character);

    const uint32_t beginAddress = CSF_AT(uint32_t, self, 0x04);
    uint32_t entryCount = 0;
    char result = 0;
    if (beginAddress)
    {
        const int32_t byteCount = static_cast<int32_t>(
            CSF_AT(uint32_t, self, 0x08) - beginAddress);
        const int64_t product = 789879043LL * byteCount;
        entryCount = static_cast<uint32_t>(byteCount / 348);
        result = static_cast<char>(product);
    }

    bool found = false;
    uint32_t entryOffset = 0;
    for (uint32_t index = 0; index < entryCount; ++index)
    {
        result = static_cast<char>(found);
        if (found)
            break;

        uint8_t* entry = reinterpret_cast<uint8_t*>(
            CSF_AT(uint32_t, self, 0x04) + entryOffset);
        const char* storedName;
        if (CSF_AT(uint32_t, entry, 0x18) < 0x10)
            storedName = reinterpret_cast<const char*>(entry + 4);
        else
            storedName = CSF_AT(const char*, entry, 0x04);

        const int comparison = __stricmp(
            storedName, reinterpret_cast<const char*>(nameBytes));
        result = static_cast<char>(comparison);
        if (!comparison)
        {
            CSF_AT(uint32_t, self, 0x10) = index;
            found = true;
        }
        entryOffset += 348;
    }

    return result;
}

HOOK(0x00782730, LevelRegistry_SetLevelName, "auto");
