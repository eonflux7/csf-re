#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, MenuEula_FindWideSubstring, 0x00532BC0, (void* self, void* , std::uint16_t* pattern, std::uint64_t packedSearch));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GetWideTextLength, 0x00695560, (std::uint16_t* text));
CSF_FUNC(void*, __fastcall, InsertWideCharacters, 0x006959F0, (void* self, void* edx, unsigned int a2, unsigned int a3, int16_t a4));
CSF_FUNC(int, __fastcall, FormatterSetPattern, 0x00697100, (void* self, void* edx, const char* pattern));
CSF_FUNC(int, __fastcall, GetIndexedTableValue_6AF8A0, 0x006AF8A0, (void* self, void* , int index));
CSF_FUNC(int*, __fastcall, DecrementTreeIterator, 0x00733C10, (void* self, void* edx));
CSF_FUNC(uint32_t*, __cdecl, AllocateTreeSentinel, 0x00733C70, ());
CSF_FUNC(char, __fastcall, DestroyTreeBranch, 0x00733D80, (void* self, void* edx, void** Block));
CSF_FUNC(void**, __fastcall, FindTreePosition, 0x00734880, (void* self, void* edx, void** a2, uint32_t* a3));
CSF_FUNC(char, __cdecl, ParseInlineTextReference, 0x00734ED0, (unsigned int* a1, void* a2, unsigned int* a3));
CSF_FUNC(int*, __fastcall, EraseTreeRange, 0x00735900, (uint32_t* a1, int a2, int* a3, int a4, int a5));
CSF_FUNC(int, __fastcall, InsertTreePosition, 0x00735ED0, (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(size_t, __cdecl, _wcslen, 0x007B0499, (const wchar_t* str));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(uint32_t, g_TextLineMetric, 0x008A88D4);

// The packet also identifies the direct wide-search target as a vtable.
static const auto kWideSearchTable = CSF_VTBL(0x00532BC0);

static wchar_t* __fastcall C_008670f0_UpdateTextLineMetric(
    C_008670f0* self, void* /*edx*/, float startArgument)
{
    uint32_t start;
    std::memcpy(&start, &startArgument, sizeof(start));
    const uint32_t textLength = self->field_0050;
    wchar_t* result = reinterpret_cast<wchar_t*>(textLength);
    if (start >= textLength)
        return result;

    alignas(8) uint8_t stateBytes[0xD0];
    S_004f21d0& state = *reinterpret_cast<S_004f21d0*>(stateBytes);
    alignas(8) uint8_t widthResultBytes[0x8];
    auto* widthResult = reinterpret_cast<uint32_t*>(widthResultBytes);

    FormatterSetPattern(stateBytes + 0x1C, nullptr, CSF_STR(0x0083AB1C, "\n"));
    FormatterSetPattern(stateBytes + 0x40, nullptr, CSF_STR(0x008671C4, "\r"));
    FormatterSetPattern(stateBytes + 0x68, nullptr, CSF_STR(0x008671C0, "@@"));
    FormatterSetPattern(stateBytes, nullptr, CSF_STR(0x008671BC, "/"));

    void* font = reinterpret_cast<void*>(self->field_0014);
    CSF_AT(float, stateBytes, 0x8C) =
        static_cast<float>(
            reinterpret_cast<double(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, font, 0)[0x1C / 4])(font, nullptr, 1));

    int reference = GetIndexedTableValue_6AF8A0(
        reinterpret_cast<void*>(self->field_0008), nullptr, 0);
    if (reference)
    {
        void* object = reinterpret_cast<void*>(reference);
        auto* dimensions =
            reinterpret_cast<uint32_t*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0)[0x40 / 4])(object, nullptr);
        CSF_AT(uint32_t, stateBytes, 0x84) = dimensions[1];
    }
    else
    {
        CSF_AT(uint32_t, stateBytes, 0x84) = 0;
    }

    auto* firstDelimiter = reinterpret_cast<uint16_t*>(state.field_0020);
    if (state.field_0034 < 8)
        firstDelimiter = reinterpret_cast<uint16_t*>(&state.field_0020);

    void* source = reinterpret_cast<uint8_t*>(self) + 0x3C;
    size_t patternLength = _wcslen(reinterpret_cast<wchar_t*>(firstDelimiter));
    uint32_t end = static_cast<uint32_t>(MenuEula_FindWideSubstring(
        source, nullptr, firstDelimiter,
        (static_cast<uint64_t>(patternLength) << 32) | start));

    auto* secondDelimiter = reinterpret_cast<uint16_t*>(state.field_0044);
    if (CSF_AT(uint32_t, stateBytes, 0x58) < 8)
        secondDelimiter = reinterpret_cast<uint16_t*>(&state.field_0044);
    patternLength = _wcslen(reinterpret_cast<wchar_t*>(secondDelimiter));
    int secondEnd = MenuEula_FindWideSubstring(
        source, nullptr, secondDelimiter,
        (static_cast<uint64_t>(patternLength) << 32) | start);
    if (end == UINT32_MAX)
        end = secondEnd == -1 ? textLength - 1 : static_cast<uint32_t>(secondEnd);

    CSF_AT(uint32_t, stateBytes, 0xA8) = 7;
    CSF_AT(uint32_t, stateBytes, 0xA4) = 0;
    CSF_AT(uint16_t, stateBytes, 0x94) = 0;
    for (uint32_t index = start; index <= end; ++index)
    {
        uint16_t* sourceData;
        if (CSF_AT(uint32_t, source, 0x18) < 8)
            sourceData = reinterpret_cast<uint16_t*>(
                reinterpret_cast<uint8_t*>(source) + 4);
        else
            sourceData = CSF_AT(uint16_t*, source, 4);
        const int16_t character = static_cast<int16_t>(sourceData[index]);

        uint8_t* begin = CSF_AT(uint8_t*, stateBytes, 0x94);
        uint8_t* insertion = begin;
        if (CSF_AT(uint32_t, stateBytes, 0xA8) < 8)
            insertion = stateBytes + 0x94;
        uint32_t insertionAddress =
            reinterpret_cast<uint32_t>(insertion) +
            2 * CSF_AT(uint32_t, stateBytes, 0xA4);
        if (CSF_AT(uint32_t, stateBytes, 0xA8) < 8)
            begin = stateBytes + 0x94;
        uint32_t insertionIndex = insertionAddress;
        if (insertionAddress)
            insertionIndex = static_cast<uint32_t>(
                static_cast<int32_t>(
                    insertionAddress - reinterpret_cast<uint32_t>(begin)) >> 1);
        InsertWideCharacters(stateBytes + 0x90, nullptr, insertionIndex, 1u, character);
    }

    bool useReferences = false;
    if (self->field_00da)
    {
        uint16_t* marker = CSF_AT(uint16_t*, stateBytes, 0x6C);
        if (CSF_AT(uint32_t, stateBytes, 0x80) < 8)
            marker = reinterpret_cast<uint16_t*>(stateBytes + 0x6C);
        patternLength = _wcslen(reinterpret_cast<wchar_t*>(marker));
        if (MenuEula_FindWideSubstring(
                stateBytes + 0x90, nullptr, marker,
                static_cast<uint64_t>(patternLength) << 32) != -1)
        {
            useReferences = GetIndexedTableValue_6AF8A0(
                reinterpret_cast<void*>(self->field_0008), nullptr, 0) != 0;
        }
    }

    if (!useReferences)
    {
        g_TextLineMetric = CSF_AT(uint32_t, stateBytes, 0x8C);
    }
    else
    {
        auto& occurrenceHead = reinterpret_cast<uint32_t&>(state.field_00c0);
        occurrenceHead = reinterpret_cast<uint32_t>(AllocateTreeSentinel());
        CSF_AT(uint8_t, reinterpret_cast<void*>(occurrenceHead), 0x11) = 1;
        CSF_AT(uint32_t, reinterpret_cast<void*>(occurrenceHead), 4) = occurrenceHead;
        CSF_AT(uint32_t, reinterpret_cast<void*>(occurrenceHead), 0) = occurrenceHead;
        CSF_AT(uint32_t, reinterpret_cast<void*>(occurrenceHead), 8) = occurrenceHead;
        state.field_00c4 = 0;

        state.field_00b4 = reinterpret_cast<uint32_t>(AllocateTreeSentinel());
        CSF_AT(uint8_t, reinterpret_cast<void*>(state.field_00b4), 0x11) = 1;
        CSF_AT(uint32_t, reinterpret_cast<void*>(state.field_00b4), 4) = state.field_00b4;
        CSF_AT(uint32_t, reinterpret_cast<void*>(state.field_00b4), 0) = state.field_00b4;
        CSF_AT(uint32_t, reinterpret_cast<void*>(state.field_00b4), 8) = state.field_00b4;
        state.field_00b8 = 0;

        DestroyTreeBranch(&state.field_00bc, nullptr,
            CSF_AT(void**, reinterpret_cast<void*>(occurrenceHead), 4));
        CSF_AT(uint32_t, reinterpret_cast<void*>(occurrenceHead), 4) = occurrenceHead;
        state.field_00c4 = 0;
        CSF_AT(uint32_t, reinterpret_cast<void*>(occurrenceHead), 0) = occurrenceHead;
        CSF_AT(uint32_t, reinterpret_cast<void*>(occurrenceHead), 8) = occurrenceHead;

        DestroyTreeBranch(&state.field_00b0, nullptr,
            CSF_AT(void**, reinterpret_cast<void*>(state.field_00b4), 4));
        CSF_AT(uint32_t, reinterpret_cast<void*>(state.field_00b4), 4) = state.field_00b4;
        state.field_00b8 = 0;
        CSF_AT(uint32_t, reinterpret_cast<void*>(state.field_00b4), 0) = state.field_00b4;
        CSF_AT(uint32_t, reinterpret_cast<void*>(state.field_00b4), 8) = state.field_00b4;

        uint16_t* marker = CSF_AT(uint16_t*, stateBytes, 0x6C);
        if (CSF_AT(uint32_t, stateBytes, 0x80) < 8)
            marker = reinterpret_cast<uint16_t*>(stateBytes + 0x6C);
        patternLength = _wcslen(reinterpret_cast<wchar_t*>(marker));
        uint32_t markerPosition = static_cast<uint32_t>(MenuEula_FindWideSubstring(
            stateBytes + 0x90, nullptr, marker,
            static_cast<uint64_t>(patternLength) << 32));
        uint32_t key = markerPosition;
        InsertTreePosition(&state.field_00bc, nullptr,
            reinterpret_cast<int>(stateBytes + 0x5C), &key);

        while (markerPosition != UINT32_MAX)
        {
            uint16_t* markerForLength = CSF_AT(uint16_t*, stateBytes, 0x6C);
            uint16_t* markerForSearch = markerForLength;
            if (CSF_AT(uint32_t, stateBytes, 0x80) < 8)
            {
                markerForLength = reinterpret_cast<uint16_t*>(stateBytes + 0x6C);
                markerForSearch = markerForLength;
            }
            uint32_t next = markerPosition + GetWideTextLength(markerForLength);
            patternLength = _wcslen(reinterpret_cast<wchar_t*>(markerForSearch));
            markerPosition = static_cast<uint32_t>(MenuEula_FindWideSubstring(
                stateBytes + 0x90, nullptr, markerForSearch,
                (static_cast<uint64_t>(patternLength) << 32) | next));
            if (markerPosition != UINT32_MAX)
            {
                key = markerPosition;
                InsertTreePosition(&state.field_00bc, nullptr,
                    reinterpret_cast<int>(stateBytes + 0x5C), &key);
            }
        }

        uint32_t length = CSF_AT(uint32_t, stateBytes, 0xA4);
        CSF_AT(uint32_t, stateBytes, 0x88) = length;
        state.field_00ac = 0;
        uint32_t position = 0;
        float measuredWidth = g_fZero;
        int lastSpace = -1;

        if (length)
        {
            do
            {
                uint32_t* availableWidth = self->vSlot5C(widthResult);
                if (static_cast<int32_t>(static_cast<int64_t>(measuredWidth)) >
                    static_cast<int32_t>(*availableWidth))
                    break;

                key = position;
                void** found = FindTreePosition(&state.field_00bc, nullptr,
                    reinterpret_cast<void**>(stateBytes + 0x60), &key);
                bool measureCharacter = false;
                uint16_t* characters = nullptr;
                if (*found == reinterpret_cast<void*>(occurrenceHead))
                {
                    characters = CSF_AT(uint16_t*, stateBytes, 0x94);
                    uint16_t* spaceData = characters;
                    if (CSF_AT(uint32_t, stateBytes, 0xA8) < 8)
                        spaceData = reinterpret_cast<uint16_t*>(stateBytes + 0x94);
                    if (spaceData[position] == 32)
                        lastSpace = static_cast<int>(position);
                    if (CSF_AT(uint32_t, stateBytes, 0xA8) < 8)
                        characters = reinterpret_cast<uint16_t*>(stateBytes + 0x94);
                    measureCharacter = true;
                }
                else
                {
                    key = position;
                    if (!ParseInlineTextReference(
                            &state.field_00ac, stateBytes + 0x90, &state.field_0064))
                    {
                        characters = CSF_AT(uint16_t*, stateBytes, 0x94);
                        if (CSF_AT(uint32_t, stateBytes, 0xA8) < 8)
                            characters = reinterpret_cast<uint16_t*>(stateBytes + 0x94);
                        position = state.field_00ac;
                        measureCharacter = true;
                    }
                    else
                    {
                        InsertTreePosition(&state.field_00b0, nullptr,
                            reinterpret_cast<int>(&state.field_0038), &key);
                        reference = GetIndexedTableValue_6AF8A0(
                            reinterpret_cast<void*>(self->field_0008), nullptr,
                            state.field_0064);
                        if (reference)
                        {
                            void* object = reinterpret_cast<void*>(reference);
                            float* dimensions =
                                reinterpret_cast<float*(__fastcall*)(void*, void*)>(
                                    CSF_AT(void**, object, 0)[0x40 / 4])(object, nullptr);
                            position = state.field_00ac;
                            measuredWidth += *dimensions;
                        }
                        else
                        {
                            position = state.field_00ac;
                            measuredWidth += g_fZero;
                        }
                    }
                }

                if (measureCharacter)
                {
                    font = reinterpret_cast<void*>(self->field_0014);
                    int glyph = reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t)>(
                        CSF_AT(void**, font, 0)[0x10 / 4])(
                            font, nullptr, characters[position]);
                    if (glyph)
                        measuredWidth +=
                            CSF_AT(int, reinterpret_cast<void*>(glyph), 0x28) *
                            self->field_00c0;
                    state.field_00ac = ++position;
                }
            }
            while (position < CSF_AT(uint32_t, stateBytes, 0x88));
        }

        if (lastSpace == -1)
        {
            length = CSF_AT(uint32_t, stateBytes, 0xA4);
            CSF_AT(uint32_t, stateBytes, 0x88) = 32;
            lastSpace = MenuEula_FindWideSubstring(
                stateBytes + 0x90, nullptr,
                reinterpret_cast<uint16_t*>(stateBytes + 0x88), 0x100000000ULL);
            if (lastSpace == -1)
                lastSpace = static_cast<int>(length);
        }

        uint32_t* availableWidth = self->vSlot5C(
            reinterpret_cast<uint32_t*>(&state.field_0038));
        void* usedHead = reinterpret_cast<void*>(state.field_00b4);
        bool chooseReferenceMetric = false;
        if (static_cast<int32_t>(static_cast<int64_t>(measuredWidth)) >
            static_cast<int32_t>(*availableWidth) && lastSpace != -1)
        {
            uint32_t iterator = CSF_AT(uint32_t, usedHead, 0);
            if (iterator != state.field_00b4)
            {
                while (CSF_AT(uint32_t, reinterpret_cast<void*>(iterator), 0xC) >
                       static_cast<uint32_t>(lastSpace))
                {
                    DecrementTreeIterator(&iterator, nullptr);
                    if (reinterpret_cast<void*>(iterator) == usedHead)
                        break;
                }
                chooseReferenceMetric = reinterpret_cast<void*>(iterator) != usedHead;
            }
        }
        else
        {
            chooseReferenceMetric = state.field_00b8 != 0;
        }

        g_TextLineMetric = CSF_AT(uint32_t, stateBytes,
            chooseReferenceMetric ? 0x84 : 0x8C);

        int eraseResult = 0;
        EraseTreeRange(reinterpret_cast<uint32_t*>(&state.field_00b0), 0,
            &eraseResult, CSF_AT(int, usedHead, 0), reinterpret_cast<int>(usedHead));
        if (state.field_00b4)
            Mem_FreeDebug(reinterpret_cast<void*>(state.field_00b4));
        state.field_00b4 = 0;
        state.field_00b8 = 0;

        EraseTreeRange(reinterpret_cast<uint32_t*>(&state.field_00bc), 0,
            &eraseResult,
            CSF_AT(int, reinterpret_cast<void*>(occurrenceHead), 0),
            static_cast<int>(occurrenceHead));
        if (occurrenceHead)
            Mem_FreeDebug(reinterpret_cast<void*>(occurrenceHead));
    }

    result = reinterpret_cast<wchar_t*>(CSF_AT(uint32_t, stateBytes, 0xA8));
    if (CSF_AT(uint32_t, stateBytes, 0xA8) >= 8)
    {
        result = CSF_AT(wchar_t*, stateBytes, 0x94);
        if (CSF_AT(uint32_t, stateBytes, 0x94))
            result = reinterpret_cast<wchar_t*>(
                Mem_FreeDebug(CSF_AT(void*, stateBytes, 0x94)));
    }
    CSF_AT(uint32_t, stateBytes, 0xA8) = 7;
    CSF_AT(uint32_t, stateBytes, 0xA4) = 0;
    CSF_AT(uint16_t, stateBytes, 0x94) = 0;

    if (state.field_0018 >= 8)
    {
        result = reinterpret_cast<wchar_t*>(state.field_0004);
        if (state.field_0004)
            result = reinterpret_cast<wchar_t*>(
                Mem_FreeDebug(reinterpret_cast<void*>(state.field_0004)));
    }
    state.field_0018 = 7;
    state.field_0014 = 0;
    state.field_0004 = 0;

    if (CSF_AT(uint32_t, stateBytes, 0x80) >= 8)
    {
        result = CSF_AT(wchar_t*, stateBytes, 0x6C);
        if (CSF_AT(uint32_t, stateBytes, 0x6C))
            result = reinterpret_cast<wchar_t*>(
                Mem_FreeDebug(CSF_AT(void*, stateBytes, 0x6C)));
    }
    CSF_AT(uint32_t, stateBytes, 0x80) = 7;
    CSF_AT(uint32_t, stateBytes, 0x7C) = 0;
    CSF_AT(uint16_t, stateBytes, 0x6C) = 0;

    if (CSF_AT(uint32_t, stateBytes, 0x58) >= 8)
    {
        result = reinterpret_cast<wchar_t*>(state.field_0044);
        if (state.field_0044)
            result = reinterpret_cast<wchar_t*>(
                Mem_FreeDebug(reinterpret_cast<void*>(state.field_0044)));
    }
    CSF_AT(uint32_t, stateBytes, 0x58) = 7;
    state.field_0054 = 0;
    reinterpret_cast<uint16_t&>(state.field_0044) = 0;

    if (state.field_0034 >= 8)
    {
        result = reinterpret_cast<wchar_t*>(state.field_0020);
        if (state.field_0020)
            return reinterpret_cast<wchar_t*>(
                Mem_FreeDebug(reinterpret_cast<void*>(state.field_0020)));
    }
    return result;
}

HOOK(0x00736250, C_008670f0_UpdateTextLineMetric, "auto");
