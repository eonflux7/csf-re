#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(const void*, __cdecl, RwImageSetPath, 0x00625270, (const void* path));
CSF_FUNC(char, __cdecl, NormalizeResourceName, 0x006949D0, (char* source, char* destination, unsigned int capacity));
CSF_FUNC(unsigned int, __cdecl, ResourceName_Transform, 0x00694B20, (char* a1, char* a2, char* a3, unsigned int a4));
CSF_FUNC(int, __fastcall, InsertUniqueUnsignedShortTreeValue, 0x0073A5C0, (void* self, void* , int resultAddress, std::uint16_t* value));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static void __cdecl FontWapa_ReadData(int* stream, void* destination, int size)
{
    reinterpret_cast<void (__fastcall*)(void*, void*, void*, int)>(
        CSF_AT(void**, stream, 0)[2])(stream, nullptr, destination, size);
}

static char __fastcall C_00867444_LoadFontVersion1(
    C_00867444* self, void* /*edx*/, int* stream)
{
    int unusedHeader = 0;
    int textureWidth = 0;
    int textureHeight = 0;
    int unusedCount = 0;
    int pageCount = 0;
    char unusedFlag = 0;

    FontWapa_ReadData(stream, &CSF_AT(std::uint32_t, self, 0x4C), 4);
    FontWapa_ReadData(stream, &unusedHeader, 4);
    FontWapa_ReadData(stream, &textureWidth, 4);
    FontWapa_ReadData(stream, &textureHeight, 4);
    FontWapa_ReadData(stream, &unusedCount, 4);
    FontWapa_ReadData(stream, &pageCount, 4);
    FontWapa_ReadData(stream, &CSF_AT(std::uint32_t, self, 0x44), 4);
    FontWapa_ReadData(stream, &CSF_AT(std::uint32_t, self, 0x48), 4);
    FontWapa_ReadData(stream, &unusedFlag, 1);

    alignas(8) std::uint8_t glyph[0x38] = {};
    CSF_AT(std::uint16_t, glyph, 0x00) = 0;
    CSF_AT(int, glyph, 0x04) = -1;
    std::memset(glyph + 0x08, 0, 16);
    CSF_AT(int, glyph, 0x18) = -1;
    CSF_AT(int, glyph, 0x1C) = -1;
    std::memset(glyph + 0x28, 255, 16);

    alignas(8) std::uint8_t insertionValue[0x3C] = {};
    alignas(8) std::uint8_t insertionResult[0x08] = {};

    for (int page = 0; page < pageCount; ++page)
    {
        int unusedPageValue1 = 0;
        int unusedPageValue2 = 0;
        unsigned int glyphCount = 0;

        FontWapa_ReadData(stream, &unusedPageValue1, 2);
        FontWapa_ReadData(stream, &unusedPageValue2, 2);
        FontWapa_ReadData(stream, &glyphCount, 4);

        for (unsigned int index = 0; index < glyphCount; ++index)
        {
            int left = 0;
            int top = 0;
            int right = 0;
            int bottom = 0;
            int metric1 = 0;
            int metric2 = 0;
            int metric3 = 0;
            int unusedMetric1 = 0;
            int unusedMetric2 = 0;
            int metric4 = 0;

            FontWapa_ReadData(stream, glyph, 2);
            FontWapa_ReadData(stream, &left, 4);
            FontWapa_ReadData(stream, &top, 4);
            FontWapa_ReadData(stream, &right, 4);
            FontWapa_ReadData(stream, &bottom, 4);
            FontWapa_ReadData(stream, &metric1, 4);
            FontWapa_ReadData(stream, &metric2, 4);
            FontWapa_ReadData(stream, &metric3, 4);
            FontWapa_ReadData(stream, &unusedMetric1, 4);
            FontWapa_ReadData(stream, &unusedMetric2, 4);
            FontWapa_ReadData(stream, &metric4, 4);

            CSF_AT(int, glyph, 0x04) = page;
            right = static_cast<int>(static_cast<std::uint32_t>(right) + 1);
            if (right > textureWidth)
                right = textureWidth;
            bottom = static_cast<int>(static_cast<std::uint32_t>(bottom) + 1);
            if (bottom > textureHeight)
                bottom = textureHeight;

            const float inverseWidth = g_TransitionOne / static_cast<float>(textureWidth);
            CSF_AT(int, glyph, 0x18) =
                static_cast<int>(static_cast<std::uint32_t>(right) -
                                 static_cast<std::uint32_t>(left));
            CSF_AT(int, glyph, 0x1C) =
                static_cast<int>(static_cast<std::uint32_t>(bottom) -
                                 static_cast<std::uint32_t>(top));
            CSF_AT(int, glyph, 0x24) = metric2;
            CSF_AT(int, glyph, 0x30) = metric2;
            CSF_AT(int, glyph, 0x20) = metric1;
            CSF_AT(int, glyph, 0x2C) = metric1;
            CSF_AT(std::uint16_t, insertionValue, 0) =
                CSF_AT(std::uint16_t, glyph, 0);
            CSF_AT(int, glyph, 0x28) = metric3;
            CSF_AT(int, glyph, 0x34) = metric4;
            CSF_AT(float, glyph, 0x08) = static_cast<float>(left) * inverseWidth;
            const float inverseHeight = g_TransitionOne / static_cast<float>(textureHeight);
            CSF_AT(float, glyph, 0x0C) = static_cast<float>(top) * inverseHeight;
            CSF_AT(float, glyph, 0x10) = static_cast<float>(right) * inverseWidth;
            CSF_AT(float, glyph, 0x14) = static_cast<float>(bottom) * inverseHeight;

            std::memcpy(insertionValue + 4, glyph, 0x38);
            InsertUniqueUnsignedShortTreeValue(
                reinterpret_cast<std::uint8_t*>(self) + 0x28, nullptr,
                reinterpret_cast<int>(insertionResult),
                reinterpret_cast<std::uint16_t*>(insertionValue));
        }
    }

    alignas(8) char texturePath[0x100] = {};
    alignas(8) char baseName[0x100] = {};
    alignas(8) char textureName[0x100] = {};

    char* resourceName;
    if (CSF_AT(std::uint32_t, self, 0x24) < 0x10)
        resourceName = reinterpret_cast<char*>(self) + 0x10;
    else
        resourceName = CSF_AT(char*, self, 0x10);
    NormalizeResourceName(resourceName, baseName, 255);

    if (CSF_AT(std::uint32_t, self, 0x24) < 0x10)
        resourceName = reinterpret_cast<char*>(self) + 0x10;
    else
        resourceName = CSF_AT(char*, self, 0x10);
    ResourceName_Transform(
        resourceName, const_cast<char*>(CSF_STR(0x00860C74, "Textures/")),
        texturePath, 256);
    RwImageSetPath(texturePath);

    const int newSize = pageCount;
    const unsigned int capacity = CSF_AT(unsigned int, self, 0x38);
    if (static_cast<unsigned int>(pageCount) > capacity)
    {
        (void)CSF_VTBL(0x0041E5F0);
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(
                reinterpret_cast<std::uint8_t*>(self) + 0x34),
            nullptr,
            static_cast<int>(static_cast<unsigned int>(pageCount) - capacity));
    }
    CSF_AT(int, self, 0x3C) = newSize;

    for (int page = 0; page < pageCount; ++page)
    {
        _sprintf(textureName, CSF_STR(0x00861DC8, "%s%d"), baseName, page);
        void* textureManager = CSF_AT(void*, self, 0x08);
        int* textureSlot = CSF_AT(int*, self, 0x34) + page;
        const int texture =
            reinterpret_cast<int (__fastcall*)(void*, void*, char*)>(
                CSF_AT(void**, textureManager, 0)[124 / 4])(
                    textureManager, nullptr, textureName);
        *textureSlot = texture;
    }
    return 1;
}

HOOK(0x0073A680, C_00867444_LoadFontVersion1, "auto");
