#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40, (std::uint32_t component));
CSF_FUNC(char, __fastcall, CopyAttributeText, 0x00692D40, (std::uint8_t* attribute, void* edx, char* buffer, int capacity));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatAttribute, 0x00693990, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __cdecl, GameBddLoader_ShouldPreloadResources, 0x0069D5C0, (int a1));
CSF_FUNC(int, __fastcall, C_00861b58_GetTwo, 0x006B3D10, (C_00861b58* , void*));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseHand, 0x007616C0, (char* String1));
CSF_FUNC(char, __fastcall, AnimationDefs_ResizeSounds, 0x00772650, (void* self, void* edx, unsigned int a2, int a3, int a4));

// Flatten the 12-byte by-value string prefix into three stack words.
// Count plus the complete 44-byte initializer gives the required ret 48 ABI.
CSF_FUNC(char, __fastcall, AnimationDefs_ResizeFiles, 0x00772FA0,
         (S_00772940* self, void* edx, unsigned int count,
          std::uint32_t stringWord0, std::uint32_t stringWord1,
          std::uint32_t stringWord2, int a4, int a5, int length,
          unsigned int capacity, int a8, void* block, int a10, int a11));

CSF_GLOBAL(const char*, g_pszKey_ID_8A9988, 0x008A9988);
CSF_GLOBAL(const char*, g_pszKey_NOMBRE_8A998C, 0x008A998C);
CSF_GLOBAL(const char*, g_pszKey_FILE_8A9998, 0x008A9998);
CSF_GLOBAL(const char*, g_pszKey_FILES, 0x008A999C);
CSF_GLOBAL(const char*, g_pszKey_LOOP_8A99A0, 0x008A99A0);
CSF_GLOBAL(const char*, g_pszKey_BLEND_IN, 0x008A99A4);
CSF_GLOBAL(const char*, g_pszKey_VEL, 0x008A99A8);
CSF_GLOBAL(const char*, g_pszKey_TRASLACION, 0x008A99AC);
CSF_GLOBAL(const char*, g_pszKey_ROTACION, 0x008A99B0);
CSF_GLOBAL(const char*, g_pszKey_SOUNDS, 0x008A99C0);
CSF_GLOBAL(const char*, g_pszKey_SND_ID, 0x008A99C4);
CSF_GLOBAL(const char*, g_pszKey_SND_TIME, 0x008A99C8);
CSF_GLOBAL(const char*, g_pszKey_MODEL3D_ITEM, 0x008A99D4);
CSF_GLOBAL(const char*, g_pszKey_MANO_ITEM, 0x008A99D8);
CSF_GLOBAL(const char*, g_pszKey_SONIDO_PC, 0x008A99DC);
CSF_GLOBAL(const char*, g_pszKey_SONIDO_PS2, 0x008A99E0);
CSF_GLOBAL(const char*, g_pszKey_SONIDO_XBOX, 0x008A99E4);

[[maybe_unused]] static auto const kChildCountTable = CSF_VTBL(0x00692B80);

static std::uint8_t* __fastcall C_00871a18_ParseAnimation(
    C_00871a18* self, void* /*edx*/, S_00692b80* source,
    int* destination, unsigned int ps2Limit, unsigned int xboxLimit)
{
    alignas(8) std::uint8_t textBytes[0x110];
    std::memset(textBytes, 0, 256);
    char* text = reinterpret_cast<char*>(textBytes);
    auto* node = reinterpret_cast<std::uint8_t*>(source);
    auto* animation = reinterpret_cast<std::uint8_t*>(destination);

    CSF_AT(int, animation, 0x00) =
        ReadIntegerAttribute(node, nullptr, const_cast<char*>(g_pszKey_ID_8A9988));
    MenuExtras_ReadText(node, nullptr, const_cast<char*>(g_pszKey_NOMBRE_8A998C),
                       text, 256);
    AssignStringRange_429B60(animation + 0x04, nullptr, text,
                            static_cast<unsigned int>(std::strlen(text)));

    CSF_AT(float, animation, 0x34) = static_cast<float>(
        ReadFloatAttribute(node, nullptr, const_cast<char*>(g_pszKey_BLEND_IN)));
    CSF_AT(float, animation, 0x30) = static_cast<float>(
        ReadFloatAttribute(node, nullptr, const_cast<char*>(g_pszKey_VEL)));
    CSF_AT(std::uint8_t, animation, 0x59) =
        ReadIntegerAttribute(node, nullptr, const_cast<char*>(g_pszKey_TRASLACION)) == 1;
    CSF_AT(std::uint8_t, animation, 0x5A) =
        ReadIntegerAttribute(node, nullptr, const_cast<char*>(g_pszKey_ROTACION)) == 1;
    CSF_AT(std::uint8_t, animation, 0x58) =
        ReadIntegerAttribute(node, nullptr, const_cast<char*>(g_pszKey_LOOP_8A99A0)) == 1;

    auto* property =
        FindProperty(node, nullptr, const_cast<char*>(g_pszKey_SONIDO_PC));
    if (property)
        CSF_AT(std::uint8_t, animation, 0x5B) =
            ParseIntegerComponent(reinterpret_cast<std::uint32_t>(property)) == 1;

    property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_SONIDO_PS2));
    if (property)
        CSF_AT(std::uint8_t, animation, 0x5C) =
            ParseIntegerComponent(reinterpret_cast<std::uint32_t>(property)) == 1;

    property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_SONIDO_XBOX));
    if (property)
        CSF_AT(std::uint8_t, animation, 0x5D) =
            ParseIntegerComponent(reinterpret_cast<std::uint32_t>(property)) == 1;

    property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_MODEL3D_ITEM));
    if (property)
    {
        CopyAttributeText(property, nullptr, text, 256);
        AssignStringRange_429B60(animation + 0x38, nullptr, text,
                                static_cast<unsigned int>(std::strlen(text)));
    }

    property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_MANO_ITEM));
    if (property)
    {
        CopyAttributeText(property, nullptr, text, 256);
        CSF_AT(int, animation, 0x54) = WeaponDefs_ParseHand(text);
    }

    auto* files = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_FILES));
    if (!files)
        return nullptr;

    unsigned int fileCount = MenuExtras_GetChildCount(files, nullptr);
    if (!fileCount)
        return nullptr;
    fileCount = MenuExtras_GetChildCount(files, nullptr);

    if (static_cast<std::uint8_t>(GameBddLoader_ShouldPreloadResources(0)))
    {
        if (fileCount >= ps2Limit)
            fileCount = ps2Limit;
    }
    else if (static_cast<std::uint8_t>(GameBddLoader_ShouldPreloadResources(1)))
    {
        if (fileCount >= xboxLimit)
            fileCount = xboxLimit;
    }

    AnimationDefs_ResizeFiles(
        reinterpret_cast<S_00772940*>(animation + 0x20), nullptr,
        fileCount, 0, 0, 0, 0, 0, 0, 0xFu, 0, nullptr, 0, 0);

    unsigned int fileIndex = 0;
    unsigned int fileOffset = 0;
    std::uint8_t* result = nullptr;
    while (fileIndex < fileCount)
    {
        auto* component = reinterpret_cast<std::uint8_t*>(
            GetComponent(files, nullptr, fileIndex));
        MenuExtras_ReadText(component, nullptr,
                           const_cast<char*>(g_pszKey_FILE_8A9998), text, 255);

        auto* file = reinterpret_cast<std::uint8_t*>(
            CSF_AT(std::uint32_t, animation, 0x24) + fileOffset);
        AssignStringRange_429B60(file, nullptr, text,
                                static_cast<unsigned int>(std::strlen(text)));

        bool loadSounds = false;
        switch (C_00861b58_GetTwo(nullptr, nullptr))
        {
        case 0:
            loadSounds = CSF_AT(std::uint8_t, animation, 0x5C) != 0;
            break;
        case 1:
            loadSounds = CSF_AT(std::uint8_t, animation, 0x5D) != 0;
            break;
        case 2:
            loadSounds = CSF_AT(std::uint8_t, animation, 0x5B) != 0;
            break;
        }

        if (!loadSounds)
        {
            loadSounds =
                reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                    self->vtbl[0x18 / 4])(self, nullptr);
        }

        if (loadSounds)
        {
            auto* sounds =
                FindProperty(component, nullptr, const_cast<char*>(g_pszKey_SOUNDS));
            if (sounds && MenuExtras_GetChildCount(sounds, nullptr))
            {
                unsigned int soundCount = MenuExtras_GetChildCount(sounds, nullptr);
                file = reinterpret_cast<std::uint8_t*>(
                    CSF_AT(std::uint32_t, animation, 0x24) + fileOffset);
                AnimationDefs_ResizeSounds(file + 0x1C, nullptr, soundCount, 0, 0);

                for (unsigned int soundIndex = 0; soundIndex < soundCount; ++soundIndex)
                {
                    auto* soundNode = reinterpret_cast<std::uint8_t*>(
                        GetComponent(sounds, nullptr, soundIndex));

                    file = reinterpret_cast<std::uint8_t*>(
                        CSF_AT(std::uint32_t, animation, 0x24) + fileOffset);
                    auto* soundEntry = reinterpret_cast<std::uint8_t*>(
                        CSF_AT(std::uint32_t, file, 0x20) + 8 * soundIndex);
                    CSF_AT(int, soundEntry, 0x00) = ReadIntegerAttribute(
                        soundNode, nullptr, const_cast<char*>(g_pszKey_SND_ID));

                    file = reinterpret_cast<std::uint8_t*>(
                        CSF_AT(std::uint32_t, animation, 0x24) + fileOffset);
                    soundEntry = reinterpret_cast<std::uint8_t*>(
                        CSF_AT(std::uint32_t, file, 0x20) + 8 * soundIndex);
                    CSF_AT(float, soundEntry, 0x04) = static_cast<float>(
                        ReadFloatAttribute(soundNode, nullptr,
                                           const_cast<char*>(g_pszKey_SND_TIME)));
                }
            }
        }

        result = reinterpret_cast<std::uint8_t*>(fileIndex + 1);
        ++fileIndex;
        fileOffset += 44;
    }
    return result;
}

HOOK(0x007737F0, C_00871a18_ParseAnimation, "auto");
