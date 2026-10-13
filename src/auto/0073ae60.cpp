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

static const auto g_ArrayCapacityVtableBinding = CSF_VTBL(0x0041E5F0);

static void GuiFont_ReadStreamBytes(int* stream, void* destination, int size)
{
    reinterpret_cast<void (__fastcall*)(void*, void*, void*, int)>(
        CSF_AT(void**, stream, 0)[8 / 4])(
            stream, nullptr, destination, size);
}

static char __fastcall C_00867444_LoadFontVersion3(
    C_00867444* self, void* /*edx*/, int* stream)
{
    int headerValue = 0;
    int textureWidth = 0;
    int textureHeight = 0;
    int extraHeaderValue = 0;
    int pageCount = 0;
    char flag1 = 0;
    char flag2 = 0;
    int trailingHeaderValue = 0;

    GuiFont_ReadStreamBytes(stream, &CSF_AT(std::uint32_t, self, 0x4C), 4);
    GuiFont_ReadStreamBytes(stream, &headerValue, 4);
    GuiFont_ReadStreamBytes(stream, &textureWidth, 4);
    GuiFont_ReadStreamBytes(stream, &textureHeight, 4);
    GuiFont_ReadStreamBytes(stream, &extraHeaderValue, 4);
    GuiFont_ReadStreamBytes(stream, &pageCount, 4);
    GuiFont_ReadStreamBytes(stream, &CSF_AT(std::uint32_t, self, 0x44), 4);
    GuiFont_ReadStreamBytes(stream, &CSF_AT(std::uint32_t, self, 0x48), 4);
    GuiFont_ReadStreamBytes(stream, &CSF_AT(std::uint8_t, self, 0x5D), 1);
    GuiFont_ReadStreamBytes(stream, &flag1, 1);
    GuiFont_ReadStreamBytes(stream, &flag2, 1);
    GuiFont_ReadStreamBytes(stream, &trailingHeaderValue, 4);

    alignas(8) std::uint8_t glyphBytes[0x38] = {};
    CSF_AT(int, glyphBytes, 0x04) = -1;
    CSF_AT(int, glyphBytes, 0x18) = -1;
    CSF_AT(int, glyphBytes, 0x1C) = -1;
    std::memset(glyphBytes + 0x28, 255, 16);

    alignas(8) std::uint8_t treeValueBytes[0x3C];
    alignas(8) std::uint8_t insertionResultBytes[0x8];

    for (int pageIndex = 0; pageIndex < pageCount; ++pageIndex)
    {
        int pageValue1 = 0;
        int pageValue2 = 0;
        unsigned int glyphCount = 0;
        GuiFont_ReadStreamBytes(stream, &pageValue1, 2);
        GuiFont_ReadStreamBytes(stream, &pageValue2, 2);
        GuiFont_ReadStreamBytes(stream, &glyphCount, 4);

        for (unsigned int glyphIndex = 0; glyphIndex < glyphCount; ++glyphIndex)
        {
            int left = 0;
            int top = 0;
            int right = 0;
            int bottom = 0;
            int bearingX = 0;
            int bearingY = 0;
            int adjustedWidth = 0;
            int unusedMetric1 = 0;
            int unusedMetric2 = 0;
            int adjustedHeight = 0;

            GuiFont_ReadStreamBytes(stream, glyphBytes, 2);
            GuiFont_ReadStreamBytes(stream, &left, 4);
            GuiFont_ReadStreamBytes(stream, &top, 4);
            GuiFont_ReadStreamBytes(stream, &right, 4);
            GuiFont_ReadStreamBytes(stream, &bottom, 4);
            GuiFont_ReadStreamBytes(stream, &bearingX, 4);
            GuiFont_ReadStreamBytes(stream, &bearingY, 4);
            GuiFont_ReadStreamBytes(stream, &adjustedWidth, 4);
            GuiFont_ReadStreamBytes(stream, &unusedMetric1, 4);
            GuiFont_ReadStreamBytes(stream, &unusedMetric2, 4);
            GuiFont_ReadStreamBytes(stream, &adjustedHeight, 4);

            CSF_AT(int, glyphBytes, 0x04) = pageIndex;
            ++right;
            if (right > textureWidth)
                right = textureWidth;
            ++bottom;
            if (bottom > textureHeight)
                bottom = textureHeight;

            if (CSF_AT(std::uint8_t, self, 0x5D))
            {
                --left;
                if (left < 0)
                    left = 0;
                --top;
                if (top < 0)
                    top = 0;
                ++adjustedWidth;
                ++adjustedHeight;
            }

            const float inverseWidth =
                g_TransitionOne / static_cast<float>(textureWidth);
            CSF_AT(int, glyphBytes, 0x18) = right - left;
            CSF_AT(int, glyphBytes, 0x1C) = bottom - top;
            CSF_AT(int, glyphBytes, 0x24) = bearingY;
            CSF_AT(int, glyphBytes, 0x30) = bearingY;
            CSF_AT(int, glyphBytes, 0x20) = bearingX;
            CSF_AT(int, glyphBytes, 0x2C) = bearingX;
            CSF_AT(std::uint16_t, treeValueBytes, 0) =
                CSF_AT(std::uint16_t, glyphBytes, 0);
            CSF_AT(int, glyphBytes, 0x28) = adjustedWidth;
            CSF_AT(int, glyphBytes, 0x34) = adjustedHeight;
            CSF_AT(float, glyphBytes, 0x08) =
                static_cast<float>(left) * inverseWidth;
            const float inverseHeight =
                g_TransitionOne / static_cast<float>(textureHeight);
            CSF_AT(float, glyphBytes, 0x0C) =
                static_cast<float>(top) * inverseHeight;
            CSF_AT(float, glyphBytes, 0x10) =
                static_cast<float>(right) * inverseWidth;
            CSF_AT(float, glyphBytes, 0x14) =
                static_cast<float>(bottom) * inverseHeight;

            CSF_AT(std::uint16_t, treeValueBytes, 0x02) = 0;
            std::memcpy(treeValueBytes + 4, glyphBytes, 56);
            InsertUniqueUnsignedShortTreeValue(
                reinterpret_cast<std::uint8_t*>(self) + 0x28,
                nullptr,
                reinterpret_cast<int>(insertionResultBytes),
                reinterpret_cast<std::uint16_t*>(treeValueBytes));
        }
    }

    alignas(8) char pathBytes[0x100] = {};
    alignas(8) char destinationBytes[0x100] = {};
    alignas(8) char bufferBytes[0x100] = {};

    char* resourceName;
    if (CSF_AT(std::uint32_t, self, 0x24) < 0x10)
        resourceName = reinterpret_cast<char*>(self) + 0x10;
    else
        resourceName = CSF_AT(char*, self, 0x10);
    NormalizeResourceName(resourceName, destinationBytes, 255);

    if (CSF_AT(std::uint32_t, self, 0x24) < 0x10)
        resourceName = reinterpret_cast<char*>(self) + 0x10;
    else
        resourceName = CSF_AT(char*, self, 0x10);
    ResourceName_Transform(
        resourceName,
        const_cast<char*>(CSF_STR(0x00860C74, "Textures/")),
        pathBytes,
        256);
    RwImageSetPath(pathBytes);

    const int storedPageCount = pageCount;
    const std::uint32_t capacity = CSF_AT(std::uint32_t, self, 0x38);
    if (static_cast<std::uint32_t>(pageCount) > capacity)
    {
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(
                reinterpret_cast<std::uint8_t*>(self) + 0x34),
            nullptr,
            static_cast<int>(static_cast<std::uint32_t>(pageCount) - capacity));
    }
    CSF_AT(std::uint32_t, self, 0x3C) =
        static_cast<std::uint32_t>(storedPageCount);

    for (int pageIndex = 0; pageIndex < pageCount; ++pageIndex)
    {
        _sprintf(bufferBytes, CSF_STR(0x00861DC8, "%s%d"),
                 destinationBytes, pageIndex);
        void* textureManager = CSF_AT(void*, self, 0x08);
        int* textureEntry = CSF_AT(int*, self, 0x34) + pageIndex;
        const int texture = reinterpret_cast<int (__fastcall*)(void*, void*, char*)>(
            CSF_AT(void**, textureManager, 0)[124 / 4])(
                textureManager, nullptr, bufferBytes);
        *textureEntry = texture;
    }
    return 1;
}

HOOK(0x0073AE60, C_00867444_LoadFontVersion3, "auto");
