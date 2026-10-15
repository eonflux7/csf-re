#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, ReserveSsoOrHeapCapacity, 0x004856E0, (void* self, void* edx, uint32_t count, uint32_t flag));
CSF_FUNC(int, __cdecl, StringFormat_GetWideTextLength, 0x00695560, (uint16_t* text));
CSF_FUNC(void*, __fastcall, InsertWideCharacters, 0x006959F0, (void* self, void* edx, unsigned int a2, unsigned int a3, int16_t a4));
CSF_FUNC(uint32_t*, __fastcall, SaveSlots_ApplyIndex, 0x00697020, (S_004f21d0* self, void* , int16_t a2));
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (S_004f21d0* self, void* , uint8_t* a2));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static int __fastcall CGuiEdit_AdjustTextRangeRight(
    CGuiEdit* self, void* /*edx*/, char forceLeftCaret)
{
    alignas(8) uint8_t firstMeasurement[0x8];
    alignas(8) uint8_t secondMeasurement[0x10];
    uint32_t& textLength = CSF_AT(uint32_t, secondMeasurement, 0x8);
    uint32_t& scratch = CSF_AT(uint32_t, secondMeasurement, 0xC);

    void* range = &self->m_wtext1;
    void* sourceString = &self->m_wtext0;
    TextInputSubobject_Init(
        reinterpret_cast<S_004f21d0*>(range), nullptr, &g_byte_82E65C);

    uint16_t* text = CSF_AT(uint32_t, range, 0x18) < 8u
        ? reinterpret_cast<uint16_t*>(static_cast<uint8_t*>(range) + 4)
        : reinterpret_cast<uint16_t*>(CSF_AT(uint32_t, range, 4));
    void* display = reinterpret_cast<void*>(self->m_pText);
    reinterpret_cast<void(__fastcall*)(void*, void*, uint16_t*)>(
        CSF_AT(void**, display, 0)[0x40 / 4])(display, nullptr, text);

    textLength = CSF_AT(uint32_t, sourceString, 0x14);
    for (uint32_t index = self->field_01ac; index < textLength; ++index)
    {
        display = reinterpret_cast<void*>(self->m_pText);
        int32_t* measured = reinterpret_cast<int32_t*(__fastcall*)(
            void*, void*, void*, int)>(
            CSF_AT(void**, display, 0)[0x78 / 4])(
                display, nullptr, secondMeasurement, 0);
        scratch = reinterpret_cast<uint32_t>(measured);

        display = reinterpret_cast<void*>(self->m_pText);
        int32_t* available = reinterpret_cast<int32_t*(__fastcall*)(
            void*, void*, void*)>(
            CSF_AT(void**, display, 0)[0x5C / 4])(
                display, nullptr, firstMeasurement);
        if (*available < *reinterpret_cast<int32_t*>(scratch))
            break;

        uint16_t* source = CSF_AT(uint32_t, sourceString, 0x18) < 8u
            ? reinterpret_cast<uint16_t*>(static_cast<uint8_t*>(sourceString) + 4)
            : reinterpret_cast<uint16_t*>(CSF_AT(uint32_t, sourceString, 4));
        SaveSlots_ApplyIndex(
            reinterpret_cast<S_004f21d0*>(range), nullptr,
            static_cast<int16_t>(source[index]));

        text = CSF_AT(uint32_t, range, 0x18) < 8u
            ? reinterpret_cast<uint16_t*>(static_cast<uint8_t*>(range) + 4)
            : reinterpret_cast<uint16_t*>(CSF_AT(uint32_t, range, 4));
        display = reinterpret_cast<void*>(self->m_pText);
        reinterpret_cast<void(__fastcall*)(void*, void*, uint16_t*)>(
            CSF_AT(void**, display, 0)[0x40 / 4])(display, nullptr, text);
    }

    display = reinterpret_cast<void*>(self->m_pText);
    int32_t* measured = reinterpret_cast<int32_t*(__fastcall*)(
        void*, void*, void*, int)>(
        CSF_AT(void**, display, 0)[0x78 / 4])(
            display, nullptr, firstMeasurement, 0);
    display = reinterpret_cast<void*>(self->m_pText);
    int32_t* available = reinterpret_cast<int32_t*(__fastcall*)(
        void*, void*, void*)>(
        CSF_AT(void**, display, 0)[0x5C / 4])(
            display, nullptr, secondMeasurement);

    if (*available < *measured)
    {
        uint32_t position = CSF_AT(uint32_t, range, 0x18) < 8u
            ? reinterpret_cast<uint32_t>(static_cast<uint8_t*>(range) + 4)
            : CSF_AT(uint32_t, range, 4);
        position += 2 * CSF_AT(uint32_t, range, 0x14) - 2;
        uint32_t begin = CSF_AT(uint32_t, range, 0x18) < 8u
            ? reinterpret_cast<uint32_t>(static_cast<uint8_t*>(range) + 4)
            : CSF_AT(uint32_t, range, 4);
        if (position)
            position = static_cast<uint32_t>(
                static_cast<int32_t>(position - begin) >> 1);
        ReserveSsoOrHeapCapacity(range, nullptr, position, 1u);
    }

    text = CSF_AT(uint32_t, range, 0x18) < 8u
        ? reinterpret_cast<uint16_t*>(static_cast<uint8_t*>(range) + 4)
        : reinterpret_cast<uint16_t*>(CSF_AT(uint32_t, range, 4));
    display = reinterpret_cast<void*>(self->m_pText);
    reinterpret_cast<void(__fastcall*)(void*, void*, uint16_t*)>(
        CSF_AT(void**, display, 0)[0x40 / 4])(display, nullptr, text);

    scratch = self->field_01ac - 1;
    while (static_cast<int32_t>(scratch) >= 0)
    {
        display = reinterpret_cast<void*>(self->m_pText);
        measured = reinterpret_cast<int32_t*(__fastcall*)(
            void*, void*, void*, int)>(
            CSF_AT(void**, display, 0)[0x78 / 4])(
                display, nullptr, firstMeasurement, 0);
        display = reinterpret_cast<void*>(self->m_pText);
        available = reinterpret_cast<int32_t*(__fastcall*)(
            void*, void*, void*)>(
            CSF_AT(void**, display, 0)[0x5C / 4])(
                display, nullptr, secondMeasurement);
        if (*available < *measured)
            break;

        uint16_t* source = CSF_AT(uint32_t, sourceString, 0x18) < 8u
            ? reinterpret_cast<uint16_t*>(static_cast<uint8_t*>(sourceString) + 4)
            : reinterpret_cast<uint16_t*>(CSF_AT(uint32_t, sourceString, 4));
        uint32_t position = CSF_AT(uint32_t, range, 0x18) < 8u
            ? reinterpret_cast<uint32_t>(static_cast<uint8_t*>(range) + 4)
            : CSF_AT(uint32_t, range, 4);
        int16_t character = static_cast<int16_t>(source[scratch]);
        uint32_t begin = CSF_AT(uint32_t, range, 0x18) < 8u
            ? reinterpret_cast<uint32_t>(static_cast<uint8_t*>(range) + 4)
            : CSF_AT(uint32_t, range, 4);
        if (position)
            position = static_cast<uint32_t>(
                static_cast<int32_t>(position - begin) >> 1);
        InsertWideCharacters(range, nullptr, position, 1u, character);

        text = CSF_AT(uint32_t, range, 0x18) < 8u
            ? reinterpret_cast<uint16_t*>(static_cast<uint8_t*>(range) + 4)
            : reinterpret_cast<uint16_t*>(CSF_AT(uint32_t, range, 4));
        display = reinterpret_cast<void*>(self->m_pText);
        reinterpret_cast<void(__fastcall*)(void*, void*, uint16_t*)>(
            CSF_AT(void**, display, 0)[0x40 / 4])(display, nullptr, text);
        --self->field_01ac;
        --scratch;
    }

    display = reinterpret_cast<void*>(self->m_pText);
    measured = reinterpret_cast<int32_t*(__fastcall*)(
        void*, void*, void*, int)>(
        CSF_AT(void**, display, 0)[0x78 / 4])(
            display, nullptr, firstMeasurement, 0);
    display = reinterpret_cast<void*>(self->m_pText);
    available = reinterpret_cast<int32_t*(__fastcall*)(
        void*, void*, void*)>(
        CSF_AT(void**, display, 0)[0x5C / 4])(
            display, nullptr, secondMeasurement);
    if (*available < *measured)
    {
        uint32_t position = CSF_AT(uint32_t, range, 0x18) < 8u
            ? reinterpret_cast<uint32_t>(static_cast<uint8_t*>(range) + 4)
            : CSF_AT(uint32_t, range, 4);
        uint32_t begin = CSF_AT(uint32_t, range, 0x18) < 8u
            ? reinterpret_cast<uint32_t>(static_cast<uint8_t*>(range) + 4)
            : CSF_AT(uint32_t, range, 4);
        if (position)
            position = static_cast<uint32_t>(
                static_cast<int32_t>(position - begin) >> 1);
        ReserveSsoOrHeapCapacity(range, nullptr, position, 1u);
        ++self->field_01ac;
    }

    text = CSF_AT(uint32_t, range, 0x18) < 8u
        ? reinterpret_cast<uint16_t*>(static_cast<uint8_t*>(range) + 4)
        : reinterpret_cast<uint16_t*>(CSF_AT(uint32_t, range, 4));
    display = reinterpret_cast<void*>(self->m_pText);
    reinterpret_cast<void(__fastcall*)(void*, void*, uint16_t*)>(
        CSF_AT(void**, display, 0)[0x40 / 4])(display, nullptr, text);

    text = CSF_AT(uint32_t, range, 0x18) < 8u
        ? reinterpret_cast<uint16_t*>(static_cast<uint8_t*>(range) + 4)
        : reinterpret_cast<uint16_t*>(CSF_AT(uint32_t, range, 4));
    int length = StringFormat_GetWideTextLength(text);
    uint32_t rangeStart = self->field_01ac;
    uint32_t rangeEnd = length == 0
        ? rangeStart
        : rangeStart + static_cast<uint32_t>(length) - 1;
    self->field_01b0 = rangeEnd;

    uint32_t caretFlag = (self->m_caretIndex < rangeStart || forceLeftCaret)
        ? 1u : (rangeEnd & 0xFFFFFF00u);
    display = reinterpret_cast<void*>(self->m_pText);
    reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
        CSF_AT(void**, display, 0)[0x8C / 4])(
            display, nullptr, caretFlag);

    display = reinterpret_cast<void*>(self->m_pText);
    bool leftCaret = reinterpret_cast<bool(__fastcall*)(void*, void*)>(
        CSF_AT(void**, display, 0)[0x88 / 4])(display, nullptr);
    uint32_t caretOffset = leftCaret
        ? 0u : self->m_caretIndex - self->field_01ac;
    display = reinterpret_cast<void*>(self->m_pText);
    return reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t)>(
        CSF_AT(void**, display, 0)[0x84 / 4])(
            display, nullptr, caretOffset);
}

HOOK(0x00721900, CGuiEdit_AdjustTextRangeRight, "auto");
