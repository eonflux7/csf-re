#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"
#include "globals.h"

CSF_FUNC(std::uint32_t*, __fastcall, InsertWideCharacter, 0x00695B30,
         (legacy::wstring* self, void* edx, std::uint32_t* result,
          void* position, std::int16_t character));

CSF_FUNC(int, __fastcall, AdjustTextRangeLeft, 0x007215F0,
         (CGuiEdit* self, void* edx, char forceHiddenCaret));

CSF_FUNC(int, __fastcall, AdjustTextRangeRight, 0x00721900,
         (CGuiEdit* self, void* edx, char forceHiddenCaret));

static int __fastcall CGuiEdit_InsertCharacter(
    CGuiEdit* self, void* /*edx*/, std::int16_t character)
{
    void* text = reinterpret_cast<void*>(self->m_pText);
    auto isCaretHidden =
        reinterpret_cast<std::uint8_t (__fastcall*)(void*, void*)>(
            CSF_AT(void**, text, 0)[0x88 / 4]);

    const std::uint32_t previousLength =
        CSF_AT(std::uint32_t, self, 0x164);
    const std::uint8_t caretHidden = isCaretHidden(text, nullptr);
    const std::uint32_t visibleStart = self->field_01ac;

    std::uint32_t insertionIndex;
    bool rebuildFromStart;

    if (visibleStart == 0)
    {
        if (caretHidden != 0)
        {
            insertionIndex = 0;
            rebuildFromStart = true;
        }
        else
        {
            const std::uint32_t caretIndex = self->field_01a8;
            const std::uint32_t visibleEnd = self->field_01b0;
            insertionIndex = caretIndex + 1;
            if (caretIndex == visibleEnd)
                self->field_01b0 = visibleEnd + 1;
            self->field_01a8 = insertionIndex;
            rebuildFromStart = false;
        }
    }
    else
    {
        const std::uint32_t visibleEnd = self->field_01b0;
        const std::uint32_t nextVisibleEnd = visibleEnd + 1;

        if (nextVisibleEnd == previousLength)
        {
            const std::uint32_t caretIndex = self->field_01a8;
            insertionIndex = caretIndex + 1;
            self->field_01a8 = insertionIndex;

            if (caretIndex < visibleStart)
            {
                rebuildFromStart = true;
            }
            else
            {
                self->field_01b0 = nextVisibleEnd;
                rebuildFromStart = false;
            }
        }
        else if (caretHidden != 0)
        {
            insertionIndex = self->field_01a8 + 1;
            self->field_01a8 = insertionIndex;
            rebuildFromStart = true;
        }
        else
        {
            const std::uint32_t caretIndex = self->field_01a8;
            insertionIndex = caretIndex + 1;
            self->field_01a8 = insertionIndex;
            if (caretIndex == visibleEnd)
                self->field_01b0 = nextVisibleEnd;
            rebuildFromStart = false;
        }
    }

    const std::uint32_t bufferAddress =
        CSF_AT(std::uint32_t, self, 0x168) < 8u
            ? static_cast<std::uint32_t>(
                  reinterpret_cast<std::uintptr_t>(
                      reinterpret_cast<std::uint8_t*>(self) + 0x154))
            : CSF_AT(std::uint32_t, self, 0x154);

    void* insertionPosition =
        reinterpret_cast<void*>(bufferAddress + insertionIndex * 2u);

    // The iterator result overwrites a four-byte argument slot in the original.
    std::uint32_t iteratorResult = static_cast<std::uint16_t>(character);
    InsertWideCharacter(&self->m_wtext0, nullptr, &iteratorResult,
                        insertionPosition, character);

    if (rebuildFromStart)
        return AdjustTextRangeRight(self, nullptr, 0);
    return AdjustTextRangeLeft(self, nullptr, 0);
}

HOOK(0x00721DC0, CGuiEdit_InsertCharacter, "auto");
