#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, ReplaceTextRange, 0x00533040, (void* self, void* edx, unsigned int a2, unsigned int a3, void* a4, unsigned int a5, unsigned int a6));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __fastcall, FormatterSetPattern, 0x00697100, (void* self, void* edx, const char* pattern));
CSF_FUNC(int, __fastcall, GetIndexedTableValue_6AF8A0, 0x006AF8A0, (void* self, void*, int index));
CSF_FUNC(char, __fastcall, GrowPendingTextItems, 0x00732D00, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, RebuildTextLayout, 0x00733630, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, FindText, 0x00733A80, (void* self, void* edx, uint16_t* a2, unsigned int a3, unsigned int a4));
CSF_FUNC(size_t, __cdecl, _wcslen, 0x007B0499, (const wchar_t* str));

CSF_GLOBAL(const char, g_FirstWrapPattern, 0x0083AB1C);
CSF_GLOBAL(const char, g_SecondWrapPattern, 0x008671C4);
CSF_GLOBAL(uint8_t*, g_PendingTextItems, 0x008A88BC);
CSF_GLOBAL(uint32_t, g_PendingTextCapacity, 0x008A88C0);
CSF_GLOBAL(uint32_t, g_PendingTextCount, 0x008A88C4);
CSF_GLOBAL(int, g_PendingTextGrowth, 0x008A88C8);
CSF_GLOBAL(uint8_t, g_PendingTextFlag, 0x008A88CC);

static void AppendPendingTextItem(int kind, uint32_t value, uint32_t position)
{
    uint32_t itemIndex = g_PendingTextCount;
    if (itemIndex >= g_PendingTextCapacity)
    {
        GrowPendingTextItems(&g_PendingTextItems, nullptr, g_PendingTextGrowth);
        itemIndex = g_PendingTextCount;
    }

    uint8_t* items = g_PendingTextItems;
    g_PendingTextCount = itemIndex + 1;
    uint8_t* item = items + 12 * itemIndex;
    CSF_AT(int, item, 0x00) = kind;
    CSF_AT(uint32_t, item, 0x04) = value;
    CSF_AT(uint32_t, item, 0x08) = position;
}

static void DestroyWrapPatterns(uint8_t* first, uint8_t* second)
{
    if (CSF_AT(uint32_t, second, 0x18) >= 8)
    {
        void* block = CSF_AT(void*, second, 0x04);
        if (block)
            Mem_FreeDebug(block);
    }

    CSF_AT(uint32_t, second, 0x18) = 7;
    CSF_AT(uint32_t, second, 0x14) = 0;
    CSF_AT(uint16_t, second, 0x04) = 0;

    if (CSF_AT(uint32_t, first, 0x18) >= 8)
    {
        void* block = CSF_AT(void*, first, 0x04);
        if (block)
            Mem_FreeDebug(block);
    }
}

static char __fastcall C_008670f0_AccumulateTextItem(
    C_008670f0* self, void* /*edx*/, wchar_t* String,
    float* accumulatedWidth, char indexedItem)
{
    alignas(8) uint8_t firstPattern[0x1C];
    alignas(8) uint8_t secondPattern[0x28];
    alignas(8) uint8_t boundsBytes[0x08];
    alignas(8) uint8_t spaceBytes[0x08];

    FormatterSetPattern(firstPattern, nullptr, &g_FirstWrapPattern);
    FormatterSetPattern(secondPattern, nullptr, &g_SecondWrapPattern);

    uint32_t* position = reinterpret_cast<uint32_t*>(String);
    int itemKind;
    uint32_t itemValue;
    uint32_t itemPosition = 0;
    float itemWidth;
    char changedLayout = 0;

    if (indexedItem)
    {
        int entry = GetIndexedTableValue_6AF8A0(
            reinterpret_cast<void*>(self->field_0008), nullptr,
            static_cast<int>(self->field_00cc));

        if (entry)
        {
            void* object = reinterpret_cast<void*>(static_cast<uintptr_t>(entry));
            float* width = reinterpret_cast<float* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0x00)[0x40 / 4])(object, nullptr);
            itemWidth = *width;
        }
        else
        {
            itemWidth = 0.0f;
        }

        itemKind = 1;
        itemValue = self->field_00cc;
    }
    else
    {
        uint16_t* characters;
        if (self->field_0054 < 8)
            characters = reinterpret_cast<uint16_t*>(&self->field_0040);
        else
            characters = reinterpret_cast<uint16_t*>(self->field_0040);

        unsigned int character = characters[*position];
        void* font = reinterpret_cast<void*>(self->field_0014);
        void* glyph = reinterpret_cast<void* (__fastcall*)(void*, void*, unsigned int)>(
            CSF_AT(void**, font, 0x00)[0x10 / 4])(font, nullptr, character);

        if (!glyph)
        {
            ++*position;
            DestroyWrapPatterns(firstPattern, secondPattern);
            return 0;
        }

        itemKind = 0;
        itemValue = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(glyph));
        itemPosition = *position;
        itemWidth = static_cast<float>(CSF_AT(int, glyph, 0x28)) * self->field_00c0;
    }

    uint32_t* bounds = self->vSlot5C(reinterpret_cast<uint32_t*>(boundsBytes));
    if (*bounds || bounds[1])
    {
        uint32_t* currentBounds =
            self->vSlot5C(reinterpret_cast<uint32_t*>(boundsBytes));
        int widthLimit = *reinterpret_cast<int*>(currentBounds);

        if (itemWidth + *accumulatedWidth <= static_cast<float>(widthLimit))
        {
            AppendPendingTextItem(itemKind, itemValue, itemPosition);
            ++*position;
            *accumulatedWidth = itemWidth + *accumulatedWidth;
        }
        else
        {
            void* text = reinterpret_cast<uint8_t*>(self) + 0x3C;

            wchar_t* firstString;
            if (CSF_AT(uint32_t, firstPattern, 0x18) < 8)
                firstString = reinterpret_cast<wchar_t*>(firstPattern + 0x04);
            else
                firstString = CSF_AT(wchar_t*, firstPattern, 0x04);

            uint32_t searchStart = *position;
            size_t length = _wcslen(firstString);
            int firstMatch = FindText(
                text, nullptr, reinterpret_cast<uint16_t*>(firstString),
                searchStart, static_cast<unsigned int>(length));

            wchar_t* secondString;
            if (CSF_AT(uint32_t, secondPattern, 0x18) < 8)
                secondString = reinterpret_cast<wchar_t*>(secondPattern + 0x04);
            else
                secondString = CSF_AT(wchar_t*, secondPattern, 0x04);

            searchStart = *position;
            length = _wcslen(secondString);
            int secondMatch = FindText(
                text, nullptr, reinterpret_cast<uint16_t*>(secondString),
                searchStart, static_cast<unsigned int>(length));

            searchStart = *position;
            CSF_AT(uint32_t, spaceBytes, 0x00) = 32;
            int spaceMatch = FindText(
                text, nullptr, reinterpret_cast<uint16_t*>(spaceBytes),
                searchStart, 1u);

            if ((firstMatch == -1 && secondMatch == -1 && spaceMatch == firstMatch)
                || firstMatch > spaceMatch
                || secondMatch > spaceMatch
                || *position == static_cast<uint32_t>(spaceMatch))
            {
                AppendPendingTextItem(itemKind, itemValue, itemPosition);
                ++*position;
                *accumulatedWidth = itemWidth + *accumulatedWidth;
            }
            else
            {
                g_PendingTextCount = 0;
                g_PendingTextFlag = 0;
                *position = static_cast<uint32_t>(spaceMatch);
                ReplaceTextRange(
                    text, nullptr, static_cast<unsigned int>(spaceMatch),
                    1u, firstPattern, 0, 0xFFFFFFFF);

                if (!self->field_00c8)
                    --self->field_0060;

                RebuildTextLayout(self, nullptr, *position);
                changedLayout = 1;
            }
        }
    }
    else
    {
        AppendPendingTextItem(itemKind, itemValue, itemPosition);
        ++*position;
    }

    DestroyWrapPatterns(firstPattern, secondPattern);
    return changedLayout;
}

HOOK(0x007353F0, C_008670f0_AccumulateTextItem, "auto");
