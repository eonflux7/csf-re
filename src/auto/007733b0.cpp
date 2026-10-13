#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(std::uint32_t, __fastcall, S_00692b80_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, S_00692b80_GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, S_00692b80_ParseIntegerComponent, 0x00692C40, (std::uint32_t component));
CSF_FUNC(char, __fastcall, CoreParser_CopyAttributeText, 0x00692D40, (std::uint8_t* attribute, void* edx, char* buffer, int capacity));
CSF_FUNC(std::uint8_t*, __fastcall, S_00692b80_FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(double, __fastcall, S_00692b80_ReadFloatAttribute, 0x00693990, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(char, __fastcall, S_00692b80_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __cdecl, GameBddLoader_ShouldPreloadResources, 0x0069D5C0, (int a1));
CSF_FUNC(int, __fastcall, C_00861b58_GetTwo, 0x006B3D10, (C_00861b58* , void*));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseHand, 0x007616C0, (char* String1));
CSF_FUNC(char, __fastcall, S_007721a0_ResizeToCount, 0x00772650, (void* self, void* edx, unsigned int a2, int a3, int a4));

// The by-value default element occupies eleven stack dwords.
// Spell them out so this binding explicitly has twelve stack arguments.
CSF_FUNC(char, __fastcall, S_00772940_ResizeRecords, 0x00772FA0,
    (void* self, void* edx, unsigned int count,
     std::uint32_t defaultHeader, std::uint32_t defaultString,
     std::uint32_t defaultStorage0, std::uint32_t defaultStorage1,
     std::uint32_t defaultStorage2, std::uint32_t defaultLength,
     unsigned int defaultCapacity, std::uint32_t defaultUnknown,
     void* defaultSoundBegin, std::uint32_t defaultSoundEnd,
     std::uint32_t defaultSoundCapacity));

CSF_GLOBAL(char*, g_pszKey_ID_8A9988, 0x008A9988);
CSF_GLOBAL(char*, g_pszKey_NOMBRE_8A998C, 0x008A998C);
CSF_GLOBAL(char*, g_pszKey_FILES, 0x008A999C);
CSF_GLOBAL(char*, g_pszKey_LOOP_8A99A0, 0x008A99A0);
CSF_GLOBAL(char*, g_pszKey_BLEND_IN, 0x008A99A4);
CSF_GLOBAL(char*, g_pszKey_VEL, 0x008A99A8);
CSF_GLOBAL(char*, g_pszKey_TRASLACION, 0x008A99AC);
CSF_GLOBAL(char*, g_pszKey_ROTACION, 0x008A99B0);
CSF_GLOBAL(char*, g_pszKey_SOUNDS, 0x008A99C0);
CSF_GLOBAL(char*, g_pszKey_SND_ID, 0x008A99C4);
CSF_GLOBAL(char*, g_pszKey_SND_TIME, 0x008A99C8);
CSF_GLOBAL(char*, g_pszKey_MODEL3D_ITEM, 0x008A99D4);
CSF_GLOBAL(char*, g_pszKey_MANO_ITEM, 0x008A99D8);
CSF_GLOBAL(char*, g_pszKey_SONIDO_PC, 0x008A99DC);
CSF_GLOBAL(char*, g_pszKey_SONIDO_PS2, 0x008A99E0);
CSF_GLOBAL(char*, g_pszKey_SONIDO_XBOX, 0x008A99E4);

static int __fastcall C_00871a18_ReadAnimationDefinition(
    C_00871a18* self,
    void* /*edx*/,
    S_00692b80* source,
    int* definition,
    unsigned int preloadLimit,
    unsigned int alternatePreloadLimit)
{
    auto* node = reinterpret_cast<std::uint8_t*>(source);
    auto* record = reinterpret_cast<std::uint8_t*>(definition);

    alignas(8) std::uint8_t textBytes[0x108];
    auto* text = reinterpret_cast<char*>(textBytes);
    std::memset(text, 0, 256);

    CSF_AT(int, record, 0x00) =
        ReadIntegerAttribute(node, nullptr, g_pszKey_ID_8A9988);
    S_00692b80_ReadText(node, nullptr, g_pszKey_NOMBRE_8A998C, text, 256);
    AssignStringRange_429B60(
        record + 0x04, nullptr, text,
        static_cast<unsigned int>(std::strlen(text)));

    auto* files = S_00692b80_FindProperty(node, nullptr, g_pszKey_FILES);
    if (files && S_00692b80_GetChildCount(files, nullptr))
    {
        unsigned int fileCount = S_00692b80_GetChildCount(files, nullptr);
        if (static_cast<std::uint8_t>(
                GameBddLoader_ShouldPreloadResources(0)))
        {
            if (fileCount >= preloadLimit)
                fileCount = preloadLimit;
        }
        else if (static_cast<std::uint8_t>(
                     GameBddLoader_ShouldPreloadResources(1)))
        {
            if (fileCount >= alternatePreloadLimit)
                fileCount = alternatePreloadLimit;
        }

        S_00772940_ResizeRecords(
            record + 0x20, nullptr, fileCount,
            0, 0, 0, 0, 0, 0, 0xF, 0, nullptr, 0, 0);

        unsigned int fileOffset = 0;
        for (unsigned int index = 0; index < fileCount; ++index)
        {
            auto* component = reinterpret_cast<std::uint8_t*>(
                S_00692b80_GetComponent(files, nullptr, index));
            CoreParser_CopyAttributeText(component, nullptr, text, 255);
            const auto length =
                static_cast<unsigned int>(std::strlen(text));
            auto* file = reinterpret_cast<std::uint8_t*>(
                CSF_AT(std::uint32_t, record, 0x24)) + fileOffset;
            AssignStringRange_429B60(file, nullptr, text, length);
            fileOffset += 44;
        }
    }

    CSF_AT(float, record, 0x34) =
        static_cast<float>(S_00692b80_ReadFloatAttribute(node, nullptr, g_pszKey_BLEND_IN));
    CSF_AT(float, record, 0x30) =
        static_cast<float>(S_00692b80_ReadFloatAttribute(node, nullptr, g_pszKey_VEL));
    CSF_AT(std::uint8_t, record, 0x59) =
        ReadIntegerAttribute(node, nullptr, g_pszKey_TRASLACION) == 1;
    CSF_AT(std::uint8_t, record, 0x5A) =
        ReadIntegerAttribute(node, nullptr, g_pszKey_ROTACION) == 1;
    CSF_AT(std::uint8_t, record, 0x58) =
        ReadIntegerAttribute(node, nullptr, g_pszKey_LOOP_8A99A0) == 1;

    auto* property = S_00692b80_FindProperty(node, nullptr, g_pszKey_SONIDO_PC);
    if (property)
    {
        CSF_AT(std::uint8_t, record, 0x5B) =
            S_00692b80_ParseIntegerComponent(
                reinterpret_cast<std::uint32_t>(property)) == 1;
    }

    property = S_00692b80_FindProperty(node, nullptr, g_pszKey_SONIDO_PS2);
    if (property)
    {
        CSF_AT(std::uint8_t, record, 0x5C) =
            S_00692b80_ParseIntegerComponent(
                reinterpret_cast<std::uint32_t>(property)) == 1;
    }

    property = S_00692b80_FindProperty(node, nullptr, g_pszKey_SONIDO_XBOX);
    if (property)
    {
        CSF_AT(std::uint8_t, record, 0x5D) =
            S_00692b80_ParseIntegerComponent(
                reinterpret_cast<std::uint32_t>(property)) == 1;
    }

    property = S_00692b80_FindProperty(node, nullptr, g_pszKey_MODEL3D_ITEM);
    if (property)
    {
        CoreParser_CopyAttributeText(property, nullptr, text, 256);
        AssignStringRange_429B60(
            record + 0x38, nullptr, text,
            static_cast<unsigned int>(std::strlen(text)));
    }

    property = S_00692b80_FindProperty(node, nullptr, g_pszKey_MANO_ITEM);
    if (property)
    {
        CoreParser_CopyAttributeText(property, nullptr, text, 256);
        CSF_AT(int, record, 0x54) = WeaponDefs_ParseHand(text);
    }

    int result = CSF_AT(int, record, 0x24);
    if (!result)
        return result;

    result = static_cast<std::int32_t>(
        CSF_AT(std::uint32_t, record, 0x28) -
        static_cast<std::uint32_t>(result)) / 44;
    if (!result)
        return result;

    bool soundEnabled = false;
    switch (C_00861b58_GetTwo(nullptr, nullptr))
    {
    case 0:
        soundEnabled = CSF_AT(std::uint8_t, record, 0x5C) != 0;
        break;
    case 1:
        soundEnabled = CSF_AT(std::uint8_t, record, 0x5D) != 0;
        break;
    case 2:
        soundEnabled = CSF_AT(std::uint8_t, record, 0x5B) != 0;
        break;
    default:
        break;
    }

    if (!soundEnabled)
    {
        result = (*reinterpret_cast<int (__fastcall***)(void*, void*)>(self))
            [0x18 / 4](self, nullptr);
        if (!static_cast<std::uint8_t>(result))
            return result;
    }

    auto* sounds = S_00692b80_FindProperty(node, nullptr, g_pszKey_SOUNDS);
    result = reinterpret_cast<int>(sounds);
    if (!sounds)
        return result;

    result = static_cast<int>(S_00692b80_GetChildCount(sounds, nullptr));
    if (!result)
        return result;

    const unsigned int soundCount =
        S_00692b80_GetChildCount(sounds, nullptr);

    const std::uint32_t filesBegin =
        CSF_AT(std::uint32_t, record, 0x24);
    const unsigned int fileCount = filesBegin
        ? static_cast<unsigned int>(static_cast<std::int32_t>(
              CSF_AT(std::uint32_t, record, 0x28) - filesBegin) / 44)
        : 0;

    unsigned int fileOffset = 0;
    for (unsigned int remaining = fileCount; remaining; --remaining)
    {
        auto* soundVector = reinterpret_cast<std::uint8_t*>(
            CSF_AT(std::uint32_t, record, 0x24)) + fileOffset + 0x1C;
        S_007721a0_ResizeToCount(
            soundVector, nullptr, soundCount, 0, 0);
        fileOffset += 44;
    }

    result = static_cast<int>(soundCount);
    for (unsigned int soundIndex = 0; soundIndex < soundCount; ++soundIndex)
    {
        auto* component = reinterpret_cast<std::uint8_t*>(
            S_00692b80_GetComponent(sounds, nullptr, soundIndex));
        const int soundId =
            ReadIntegerAttribute(component, nullptr, g_pszKey_SND_ID);
        const float soundTime = static_cast<float>(
            S_00692b80_ReadFloatAttribute(component, nullptr, g_pszKey_SND_TIME));

        const unsigned int soundOffset = 8 * soundIndex;
        unsigned int entryOffset = 0;
        for (unsigned int remaining = fileCount; remaining; --remaining)
        {
            auto* file = reinterpret_cast<std::uint8_t*>(
                CSF_AT(std::uint32_t, record, 0x24)) + entryOffset;
            auto* events = reinterpret_cast<std::uint8_t*>(
                CSF_AT(std::uint32_t, file, 0x20));
            CSF_AT(int, events + soundOffset, 0x00) = soundId;

            file = reinterpret_cast<std::uint8_t*>(
                CSF_AT(std::uint32_t, record, 0x24)) + entryOffset;
            events = reinterpret_cast<std::uint8_t*>(
                CSF_AT(std::uint32_t, file, 0x20));
            CSF_AT(float, events + soundOffset, 0x04) = soundTime;
            entryOffset += 44;
        }
        result = static_cast<int>(soundIndex + 1);
    }

    return result;
}

HOOK(0x007733B0, C_00871a18_ReadAnimationDefinition, "auto");
