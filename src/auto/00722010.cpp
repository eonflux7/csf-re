#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, SaveGame_DestroyLocalizedString, 0x00428E90, (void* self, void*));
CSF_FUNC(void*, __fastcall, ReserveSsoOrHeapCapacity, 0x004856E0, (void* self, void* edx, uint32_t count, uint32_t flag));
CSF_FUNC(void*, __fastcall, GuiEdit_AppendRepeatedWideCharacter, 0x00541150, (void* self, void* edx, unsigned int a2, int16_t a3));
CSF_FUNC(int, __cdecl, StringFormat_GetWideTextLength, 0x00695560, (uint16_t* text));
CSF_FUNC(std::uint32_t*, __fastcall, S_004f21d0_AssignText, 0x00696F90, (std::uint32_t* self, void* edx, std::int16_t* text));
CSF_FUNC(int, __fastcall, CGuiSlider_IsSelectableAndNotType10, 0x00718700, (CGuiSlider* self, void*));
CSF_FUNC(void, __fastcall, CGuiControl_PrepareListRendering, 0x00719A20, (void* self, void* edx));

CSF_GLOBAL(float, g_EditCaretMinimumOffset, 0x00864E10);

static void __fastcall CGuiEdit_Render(CGuiEdit* self, void* /*edx*/)
{
    if (self->vIsDisabled())
        return;

    CGuiControl_PrepareListRendering(self, nullptr);
    if (!self->m_pText)
        return;

    alignas(8) uint8_t positionBytes[0x8];
    alignas(8) uint8_t absolutePositionBytes[0x8];
    alignas(8) uint8_t caretPositionBytes[0x8];
    alignas(8) uint8_t textColorBytes[0x10];
    alignas(8) uint8_t fadedTextColorBytes[0x10];
    alignas(8) uint8_t caretColorBytes[0x10];
    alignas(8) uint8_t fadedCaretColorBytes[0x10];

    void* text = reinterpret_cast<void*>(self->m_pText);
    void** textVtable = CSF_AT(void**, text, 0);
    int* absolutePosition =
        self->vGetAbsolutePosition(reinterpret_cast<int*>(positionBytes));
    text = reinterpret_cast<void*>(self->m_pText);
    reinterpret_cast<void(__fastcall*)(void*, void*, int*)>(
        textVtable[0x50 / 4])(text, nullptr, absolutePosition);

    text = reinterpret_cast<void*>(self->m_pText);
    textVtable = CSF_AT(void**, text, 0);
    void* size = self->vGetSize();
    text = reinterpret_cast<void*>(self->m_pText);
    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        textVtable[0x58 / 4])(text, nullptr, size);

    if (self->field_0149)
    {
        text = reinterpret_cast<void*>(self->m_pText);
        int16_t* originalText =
            reinterpret_cast<int16_t*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, text, 0)[0x44 / 4])(text, nullptr);
        S_004f21d0_AssignText(
            reinterpret_cast<uint32_t*>(&self->m_wtext2), nullptr, originalText);

        // The local VC7.1 wide string occupies the whole -0x5C..-0x40 region.
        alignas(8) uint8_t maskedTextBytes[0x1C];
        CSF_AT(uint32_t, maskedTextBytes, 0x18) = 7;
        CSF_AT(uint32_t, maskedTextBytes, 0x14) = 0;
        CSF_AT(uint16_t, maskedTextBytes, 0x4) = 0;
        ReserveSsoOrHeapCapacity(maskedTextBytes, nullptr, 0, 0xFFFFFFFF);

        uint16_t* savedText =
            CSF_AT(uint32_t, &self->m_wtext2, 0x18) < 8u
                ? reinterpret_cast<uint16_t*>(
                      reinterpret_cast<uint8_t*>(&self->m_wtext2) + 4)
                : CSF_AT(uint16_t*, &self->m_wtext2, 4);
        uint32_t length =
            static_cast<uint32_t>(StringFormat_GetWideTextLength(savedText));
        uint32_t currentLength = CSF_AT(uint32_t, maskedTextBytes, 0x14);
        if (length > currentLength)
            GuiEdit_AppendRepeatedWideCharacter(
                maskedTextBytes, nullptr, length - currentLength, 42);
        else
            ReserveSsoOrHeapCapacity(
                maskedTextBytes, nullptr, length, 0xFFFFFFFF);

        uint16_t* maskedText =
            CSF_AT(uint32_t, maskedTextBytes, 0x18) < 8u
                ? reinterpret_cast<uint16_t*>(maskedTextBytes + 4)
                : CSF_AT(uint16_t*, maskedTextBytes, 4);
        text = reinterpret_cast<void*>(self->m_pText);
        reinterpret_cast<void(__fastcall*)(void*, void*, uint16_t*)>(
            CSF_AT(void**, text, 0)[0x40 / 4])(text, nullptr, maskedText);
        SaveGame_DestroyLocalizedString(maskedTextBytes, nullptr);
    }

    text = reinterpret_cast<void*>(self->m_pText);
    void* textColor =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, text, 0)[0x64 / 4])(text, nullptr);
    CSF_AT(uint32_t, textColorBytes, 0) = CSF_AT(uint32_t, textColor, 0);
    CSF_AT(uint32_t, textColorBytes, 4) = CSF_AT(uint32_t, textColor, 4);
    CSF_AT(uint32_t, textColorBytes, 8) = CSF_AT(uint32_t, textColor, 8);
    CSF_AT(float, textColorBytes, 0xC) = CSF_AT(float, textColor, 0xC);
    CSF_AT(uint32_t, fadedTextColorBytes, 0) =
        CSF_AT(uint32_t, textColorBytes, 0);
    CSF_AT(uint32_t, fadedTextColorBytes, 4) =
        CSF_AT(uint32_t, textColorBytes, 4);
    CSF_AT(uint32_t, fadedTextColorBytes, 8) =
        CSF_AT(uint32_t, textColorBytes, 8);
    CSF_AT(float, fadedTextColorBytes, 0xC) =
        CSF_AT(float, textColorBytes, 0xC) * self->field_014c;

    text = reinterpret_cast<void*>(self->m_pText);
    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, text, 0)[0x60 / 4])(text, nullptr, fadedTextColorBytes);
    self->vDispatchStateDependentAction(reinterpret_cast<void*>(self->m_pText));
    text = reinterpret_cast<void*>(self->m_pText);
    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, text, 0)[0x60 / 4])(text, nullptr, textColorBytes);

    if (self->field_0149)
    {
        uint16_t* savedText =
            CSF_AT(uint32_t, &self->m_wtext2, 0x18) < 8u
                ? reinterpret_cast<uint16_t*>(
                      reinterpret_cast<uint8_t*>(&self->m_wtext2) + 4)
                : CSF_AT(uint16_t*, &self->m_wtext2, 4);
        text = reinterpret_cast<void*>(self->m_pText);
        reinterpret_cast<void(__fastcall*)(void*, void*, uint16_t*)>(
            CSF_AT(void**, text, 0)[0x40 / 4])(text, nullptr, savedText);
    }

    if (!self->field_0138 || !self->field_013c)
        return;
    if (!self->field_0148 && !self->vIsSelected())
        return;
    if (!static_cast<uint8_t>(self->vIsShown()))
        return;
    if (!static_cast<uint8_t>(CGuiSlider_IsSelectableAndNotType10(
            reinterpret_cast<CGuiSlider*>(self), nullptr)))
        return;

    text = reinterpret_cast<void*>(self->m_pText);
    void* font =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, text, 0)[0x28 / 4])(text, nullptr);
    if (!font)
        return;

    text = reinterpret_cast<void*>(self->m_pText);
    void* offset =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, text, 0)[0x7C / 4])(text, nullptr);
    int32_t offsetY =
        static_cast<int32_t>(static_cast<int64_t>(CSF_AT(float, offset, 4)));

    text = reinterpret_cast<void*>(self->m_pText);
    offset = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, text, 0)[0x7C / 4])(text, nullptr);
    int32_t offsetX =
        static_cast<int32_t>(static_cast<int64_t>(CSF_AT(float, offset, 0)));
    if (!(static_cast<float>(offsetX) > g_EditCaretMinimumOffset))
        return;

    absolutePosition =
        self->vGetAbsolutePosition(reinterpret_cast<int*>(absolutePositionBytes));
    CSF_AT(int32_t, caretPositionBytes, 4) = static_cast<int32_t>(
        static_cast<uint32_t>(absolutePosition[1]) +
        static_cast<uint32_t>(offsetY));
    CSF_AT(int32_t, caretPositionBytes, 0) = static_cast<int32_t>(
        static_cast<uint32_t>(absolutePosition[0]) +
        static_cast<uint32_t>(offsetX));

    int32_t fontHeight = static_cast<int32_t>(static_cast<int64_t>(
        reinterpret_cast<double(__fastcall*)(void*, void*)>(
            CSF_AT(void**, font, 0)[0x24 / 4])(font, nullptr)));
    if (fontHeight == -1)
        return;

    void* caret = reinterpret_cast<void*>(self->field_0138);
    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, caret, 0)[0x1C / 4])(caret, nullptr, caretPositionBytes);

    CSF_AT(int32_t, positionBytes, 0) =
        CSF_AT(int32_t, caretPositionBytes, 0);
    CSF_AT(int32_t, positionBytes, 4) = static_cast<int32_t>(
        CSF_AT(uint32_t, caretPositionBytes, 4) -
        static_cast<uint32_t>(fontHeight));
    caret = reinterpret_cast<void*>(self->field_0138);
    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, caret, 0)[0x24 / 4])(caret, nullptr, positionBytes);

    caret = reinterpret_cast<void*>(self->field_0138);
    void* caretColor =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, caret, 0)[0x30 / 4])(caret, nullptr);
    CSF_AT(uint32_t, caretColorBytes, 0) = CSF_AT(uint32_t, caretColor, 0);
    CSF_AT(uint32_t, caretColorBytes, 4) = CSF_AT(uint32_t, caretColor, 4);
    CSF_AT(uint32_t, caretColorBytes, 8) = CSF_AT(uint32_t, caretColor, 8);
    CSF_AT(float, caretColorBytes, 0xC) = CSF_AT(float, caretColor, 0xC);
    CSF_AT(uint32_t, fadedCaretColorBytes, 0) =
        CSF_AT(uint32_t, caretColorBytes, 0);
    CSF_AT(uint32_t, fadedCaretColorBytes, 4) =
        CSF_AT(uint32_t, caretColorBytes, 4);
    CSF_AT(uint32_t, fadedCaretColorBytes, 8) =
        CSF_AT(uint32_t, caretColorBytes, 8);
    CSF_AT(float, fadedCaretColorBytes, 0xC) =
        CSF_AT(float, caretColorBytes, 0xC) * self->field_014c;

    caret = reinterpret_cast<void*>(self->field_0138);
    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, caret, 0)[0x2C / 4])(caret, nullptr, fadedCaretColorBytes);
    caret = reinterpret_cast<void*>(self->field_0138);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, caret, 0)[0x8 / 4])(caret, nullptr);
    caret = reinterpret_cast<void*>(self->field_0138);
    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, caret, 0)[0x2C / 4])(caret, nullptr, caretColorBytes);
}

HOOK(0x00722010, CGuiEdit_Render, "auto");
