#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(const void*, __cdecl, RwImageSetPath, 0x00625270, (const void* path));
CSF_FUNC(char, __cdecl, CoreString_NormalizeResourceName, 0x006949D0, (char* source, char* destination, unsigned int capacity));
CSF_FUNC(unsigned int, __cdecl, CoreString_ExtractResourcePathAndAppend, 0x00694B20, (char* a1, char* a2, char* a3, unsigned int a4));
CSF_FUNC(int, __fastcall, InsertUniqueUnsignedShortTreeValue, 0x0073A5C0, (void* self, void* , int resultAddress, std::uint16_t* value));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static void FontWapa_ReadBytes(int* stream, void* destination, int size)
{
    reinterpret_cast<void(__fastcall*)(void*, void*, void*, int)>(
        CSF_AT(void**, stream, 0)[0x8 / 4])(
            stream, nullptr, destination, size);
}

static char __fastcall C_00867444_LoadFontVersion2(
    C_00867444* self, void* /*edx*/, int* stream)
{
    int headerValue = 0;
    int textureWidth = 0;
    int textureHeight = 0;
    int anotherHeaderValue = 0;
    int textureCount = 0;
    char flag1 = 0;
    char flag2 = 0;
    int trailingHeaderValue = 0;

    FontWapa_ReadBytes(stream, &self->field_004c, 4);
    FontWapa_ReadBytes(stream, &headerValue, 4);
    FontWapa_ReadBytes(stream, &textureWidth, 4);
    FontWapa_ReadBytes(stream, &textureHeight, 4);
    FontWapa_ReadBytes(stream, &anotherHeaderValue, 4);
    FontWapa_ReadBytes(stream, &textureCount, 4);
    FontWapa_ReadBytes(stream, &self->field_0044, 4);
    FontWapa_ReadBytes(stream, &self->field_0048, 4);
    FontWapa_ReadBytes(stream, &flag1, 1);
    FontWapa_ReadBytes(stream, &flag2, 1);
    FontWapa_ReadBytes(stream, &trailingHeaderValue, 4);

    alignas(8) std::uint8_t glyph[0x38] = {};
    CSF_AT(int, glyph, 0x04) = -1;
    CSF_AT(int, glyph, 0x18) = -1;
    CSF_AT(int, glyph, 0x1c) = -1;
    std::memset(glyph + 0x28, 255, 16);
    CSF_AT(std::uint16_t, glyph, 0) = 0;
    std::memset(glyph + 0x08, 0, 16);

    for (int page = 0; page < textureCount; ++page)
    {
        int pageValue1 = 0;
        int pageValue2 = 0;
        unsigned int glyphCount = 0;
        FontWapa_ReadBytes(stream, &pageValue1, 2);
        FontWapa_ReadBytes(stream, &pageValue2, 2);
        FontWapa_ReadBytes(stream, &glyphCount, 4);

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

            FontWapa_ReadBytes(stream, glyph, 2);
            FontWapa_ReadBytes(stream, &left, 4);
            FontWapa_ReadBytes(stream, &top, 4);
            FontWapa_ReadBytes(stream, &right, 4);
            FontWapa_ReadBytes(stream, &bottom, 4);
            FontWapa_ReadBytes(stream, &metric1, 4);
            FontWapa_ReadBytes(stream, &metric2, 4);
            FontWapa_ReadBytes(stream, &metric3, 4);
            FontWapa_ReadBytes(stream, &unusedMetric1, 4);
            FontWapa_ReadBytes(stream, &unusedMetric2, 4);
            FontWapa_ReadBytes(stream, &metric4, 4);

            CSF_AT(int, glyph, 0x04) = page;
            right = static_cast<int>(static_cast<std::uint32_t>(right) + 1);
            if (right > textureWidth)
                right = textureWidth;
            bottom = static_cast<int>(static_cast<std::uint32_t>(bottom) + 1);
            if (bottom > textureHeight)
                bottom = textureHeight;

            CSF_AT(int, glyph, 0x18) =
                static_cast<int>(static_cast<std::uint32_t>(right) -
                                 static_cast<std::uint32_t>(left));
            CSF_AT(int, glyph, 0x1c) =
                static_cast<int>(static_cast<std::uint32_t>(bottom) -
                                 static_cast<std::uint32_t>(top));
            CSF_AT(int, glyph, 0x24) = metric2;
            CSF_AT(int, glyph, 0x30) = metric2;
            CSF_AT(int, glyph, 0x20) = metric1;
            CSF_AT(int, glyph, 0x2c) = metric1;
            CSF_AT(int, glyph, 0x28) = metric3;
            CSF_AT(int, glyph, 0x34) = metric4;

            // The original keeps the quotient in an x87 register (extended precision) and
            // rounds only each product to float; compute in double to match.
            const double inverseWidth =
                static_cast<double>(g_TransitionOne) / static_cast<double>(textureWidth);
            const double inverseHeight =
                static_cast<double>(g_TransitionOne) / static_cast<double>(textureHeight);
            CSF_AT(float, glyph, 0x08) = static_cast<float>(static_cast<double>(left) * inverseWidth);
            CSF_AT(float, glyph, 0x0c) = static_cast<float>(static_cast<double>(top) * inverseHeight);
            CSF_AT(float, glyph, 0x10) = static_cast<float>(static_cast<double>(right) * inverseWidth);
            CSF_AT(float, glyph, 0x14) = static_cast<float>(static_cast<double>(bottom) * inverseHeight);

            alignas(8) std::uint8_t entry[0x3c] = {};
            alignas(8) std::uint8_t insertionResult[0x08];
            CSF_AT(std::uint16_t, entry, 0) = CSF_AT(std::uint16_t, glyph, 0);
            std::memcpy(entry + 4, glyph, sizeof(glyph));
            // Tree head is at self+0x28; the original also passes the entry pointer in edx.
            InsertUniqueUnsignedShortTreeValue(
                reinterpret_cast<std::uint8_t*>(self) + 0x28,
                reinterpret_cast<void*>(entry),
                static_cast<int>(reinterpret_cast<std::uintptr_t>(insertionResult)),
                reinterpret_cast<std::uint16_t*>(entry));
        }
    }

    alignas(8) char texturePath[0x100] = {};
    alignas(8) char resourceName[0x100] = {};
    alignas(8) char textureName[0x100] = {};

    char* source = self->field_0024 < 0x10
        ? reinterpret_cast<char*>(&self->field_0010)
        : reinterpret_cast<char*>(self->field_0010);
    CoreString_NormalizeResourceName(source, resourceName, 255);

    source = self->field_0024 < 0x10
        ? reinterpret_cast<char*>(&self->field_0010)
        : reinterpret_cast<char*>(self->field_0010);
    CoreString_ExtractResourcePathAndAppend(
        source, const_cast<char*>(CSF_STR(0x00860C74, "Textures/")),
        texturePath, 256);
    RwImageSetPath(texturePath);

    // This address is also classified as a vtable reference in the packet.
    (void)CSF_VTBL(0x0041E5F0);
    if (static_cast<unsigned int>(textureCount) > self->field_0038)
    {
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(
                reinterpret_cast<std::uint8_t*>(self) + 0x34),
            nullptr,
            static_cast<int>(
                static_cast<unsigned int>(textureCount) - self->field_0038));
    }
    self->field_003c = static_cast<std::uint32_t>(textureCount);

    for (int page = 0; page < textureCount; ++page)
    {
        _sprintf(textureName, CSF_STR(0x00861DC8, "%s%d"), resourceName, page);
        void* textureManager = reinterpret_cast<void*>(self->field_0008);
        int* destination = reinterpret_cast<int*>(self->field_0034) + page;
        int texture = reinterpret_cast<int(__fastcall*)(void*, void*, char*)>(
            CSF_AT(void**, textureManager, 0)[0x7c / 4])(
                textureManager, nullptr, textureName);
        *destination = texture;
    }
    return 1;
}

HOOK(0x0073AA60, C_00867444_LoadFontVersion2, "auto");
