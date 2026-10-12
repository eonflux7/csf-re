#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(char, __fastcall, CopyAttributeText, 0x00692D40, (std::uint8_t* attribute, void* edx, char* buffer, int capacity));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedElement, 0x00693360, (uint8_t* self, void* edx, const char* name));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatAttribute, 0x00693990, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __cdecl, GameBddLoader_ShouldPreloadResources, 0x0069D5C0, (int a1));
CSF_FUNC(int, __fastcall, AnimationDefs_ResizeAnimations, 0x00774160, (void* self, void* edx, unsigned int a2));

// Flatten the three-dword by-value string header so the binding explicitly
// describes all twelve stack words consumed by the callee's ret 48.
CSF_FUNC(char, __fastcall, AnimationDefs_ResizeFiles, 0x00772FA0,
         (S_00772940* self, void* edx, unsigned int count,
          std::uint32_t stringWord0, std::uint32_t stringWord1,
          std::uint32_t stringWord2, int a4, int a5, int a6,
          unsigned int stringCapacity, int a8, void* block, int a10, int a11));

CSF_GLOBAL(const char*, g_pszKey_LISTADATOS_8A9984, 0x008A9984);
CSF_GLOBAL(const char*, g_pszKey_ID_8A9988, 0x008A9988);
CSF_GLOBAL(const char*, g_pszKey_NOMBRE_8A998C, 0x008A998C);
CSF_GLOBAL(const char*, g_pszKey_FILES, 0x008A999C);
CSF_GLOBAL(const char*, g_pszKey_LOOP_8A99A0, 0x008A99A0);
CSF_GLOBAL(const char*, g_pszKey_BLEND_IN, 0x008A99A4);
CSF_GLOBAL(const char*, g_pszKey_VEL, 0x008A99A8);
CSF_GLOBAL(const char*, g_pszKey_TRASLACION, 0x008A99AC);
CSF_GLOBAL(const char*, g_pszKey_ROTACION, 0x008A99B0);

static char __fastcall AnimationDefs_LoadAnimations(
    void* self, void* /*edx*/, S_00692b80* source)
{
    alignas(8) std::uint8_t bufferBytes[0x120];
    char* buffer = reinterpret_cast<char*>(bufferBytes);
    std::memset(buffer, 0, 256);

    // The packet also labels the child-count entry point as a vtable address.
    (void)CSF_VTBL(0x00692B80);

    std::uint8_t* list = LookupNamedElement(
        reinterpret_cast<std::uint8_t*>(source), nullptr,
        g_pszKey_LISTADATOS_8A9984);
    const unsigned int animationCount = MenuExtras_GetChildCount(list, nullptr);
    AnimationDefs_ResizeAnimations(
        CSF_AT(void*, self, 0x04), nullptr, animationCount);

    bool preload =
        static_cast<std::uint8_t>(GameBddLoader_ShouldPreloadResources(0)) != 0;
    if (!preload)
        preload =
            static_cast<std::uint8_t>(GameBddLoader_ShouldPreloadResources(1)) != 0;

    unsigned int recordOffset = 0;
    for (unsigned int index = 0; index < animationCount; ++index)
    {
        std::uint8_t* component = reinterpret_cast<std::uint8_t*>(
            GetComponent(list, nullptr, index));

        std::uint8_t* record =
            CSF_AT(std::uint8_t*, CSF_AT(void*, self, 0x04), 0x04) +
            recordOffset;
        int id = ReadIntegerAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_ID_8A9988));
        CSF_AT(int, record, 0x00) = id;

        MenuExtras_ReadText(
            component, nullptr, const_cast<char*>(g_pszKey_NOMBRE_8A998C),
            buffer, 256);
        record = CSF_AT(std::uint8_t*, CSF_AT(void*, self, 0x04), 0x04) +
                 recordOffset;
        AssignStringRange_429B60(
            record + 0x04, nullptr, buffer,
            static_cast<unsigned int>(std::strlen(buffer)));

        std::uint8_t* files = FindProperty(
            component, nullptr, const_cast<char*>(g_pszKey_FILES));
        if (files && MenuExtras_GetChildCount(files, nullptr))
        {
            unsigned int fileCount = MenuExtras_GetChildCount(files, nullptr);
            if (preload)
                fileCount = 1;

            record = CSF_AT(std::uint8_t*, CSF_AT(void*, self, 0x04), 0x04) +
                     recordOffset;
            AnimationDefs_ResizeFiles(
                reinterpret_cast<S_00772940*>(record + 0x20), nullptr,
                fileCount, 0, 0, 0, 0, 0, 0, 0xFu, 0, nullptr, 0, 0);

            unsigned int fileOffset = 0;
            for (unsigned int fileIndex = 0; fileIndex < fileCount; ++fileIndex)
            {
                std::uint8_t* attribute = reinterpret_cast<std::uint8_t*>(
                    GetComponent(files, nullptr, fileIndex));
                CopyAttributeText(attribute, nullptr, buffer, 255);

                record =
                    CSF_AT(std::uint8_t*, CSF_AT(void*, self, 0x04), 0x04) +
                    recordOffset;
                const unsigned int length =
                    static_cast<unsigned int>(std::strlen(buffer));
                AssignStringRange_429B60(
                    CSF_AT(std::uint8_t*, record, 0x24) + fileOffset,
                    nullptr, buffer, length);
                fileOffset += 44;
            }
        }

        record = CSF_AT(std::uint8_t*, CSF_AT(void*, self, 0x04), 0x04) +
                 recordOffset;
        double blendIn = ReadFloatAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_BLEND_IN));
        CSF_AT(float, record, 0x34) = static_cast<float>(blendIn);

        record = CSF_AT(std::uint8_t*, CSF_AT(void*, self, 0x04), 0x04) +
                 recordOffset;
        double velocity = ReadFloatAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_VEL));
        CSF_AT(float, record, 0x30) = static_cast<float>(velocity);

        record = CSF_AT(std::uint8_t*, CSF_AT(void*, self, 0x04), 0x04) +
                 recordOffset;
        int translation = ReadIntegerAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_TRASLACION));
        CSF_AT(std::uint8_t, record, 0x59) = translation == 1;

        record = CSF_AT(std::uint8_t*, CSF_AT(void*, self, 0x04), 0x04) +
                 recordOffset;
        int rotation = ReadIntegerAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_ROTACION));
        CSF_AT(std::uint8_t, record, 0x5A) = rotation == 1;

        record = CSF_AT(std::uint8_t*, CSF_AT(void*, self, 0x04), 0x04) +
                 recordOffset;
        int loop = ReadIntegerAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_LOOP_8A99A0));
        CSF_AT(std::uint8_t, record, 0x58) = loop == 1;

        recordOffset += 96;
    }

    return 1;
}

HOOK(0x00774680, AnimationDefs_LoadAnimations, "auto");
