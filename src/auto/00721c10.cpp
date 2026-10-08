#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(std::uint32_t*, __fastcall, MenuEula_ComputeWriteCursorForEndPointer, 0x00532CB0,
         (legacy::wstring* self, void* edx, std::uint32_t* result,
          std::uint16_t* position));

// The apparent EBX arguments are actually saves of a nonvolatile register.
// Each routine takes only one stack argument.
CSF_FUNC(int, __fastcall, AdjustTextRangeLeft, 0x007215F0,
         (CGuiEdit* self, void* edx, char atStart));
CSF_FUNC(int, __fastcall, AdjustTextRangeRight, 0x00721900,
         (CGuiEdit* self, void* edx, char atStart));

static std::uint16_t* __fastcall CGuiEdit_GetWideCharacterAddress(
    CGuiEdit* self, void* /*edx*/, std::uint32_t index)
{
    std::uintptr_t bufferAddress;
    if (CSF_AT(std::uint32_t, self, 0x168) < 8u)
    {
        bufferAddress = reinterpret_cast<std::uintptr_t>(
            reinterpret_cast<std::uint8_t*>(self) + 0x154);
    }
    else
    {
        bufferAddress = CSF_AT(std::uint32_t, self, 0x154);
    }

    return reinterpret_cast<std::uint16_t*>(bufferAddress + index * 2u);
}

static void __fastcall CGuiEdit_DeleteCharacter(CGuiEdit* self, void* /*edx*/)
{
    std::uint32_t erasedIterator;
    const std::uint32_t textLength = CSF_AT(std::uint32_t, self, 0x164);
    if (textLength == 0)
        return;

    void* const textObject = reinterpret_cast<void*>(self->m_pText);
    void** const textVtable = CSF_AT(void**, textObject, 0);
    const std::uint32_t originalIndex = self->field_01a8;
    const std::uint8_t textState = static_cast<std::uint8_t>(
        ((int(__fastcall*)(void*, void*))textVtable[0x88 / 4])(
            textObject, nullptr));

    const std::uint32_t visibleStart = self->field_01ac;
    if (visibleStart == 0)
    {
        const std::uint32_t currentIndex = self->field_01a8;
        if (currentIndex != 0)
        {
            self->field_01a8 = currentIndex - 1u;
            MenuEula_ComputeWriteCursorForEndPointer(
                &self->m_wtext0, nullptr, &erasedIterator,
                CGuiEdit_GetWideCharacterAddress(self, nullptr, originalIndex));
            AdjustTextRangeRight(self, nullptr, 0);
        }
        else
        {
            if (textState == 0)
            {
                MenuEula_ComputeWriteCursorForEndPointer(
                    &self->m_wtext0, nullptr, &erasedIterator,
                    CGuiEdit_GetWideCharacterAddress(
                        self, nullptr, originalIndex));
            }
            AdjustTextRangeRight(self, nullptr, 1);
        }
        return;
    }

    const std::uint32_t visibleEnd = self->field_01b0;
    if (visibleEnd + 1u == textLength)
    {
        if (visibleEnd != 0)
            self->field_01b0 = visibleEnd - 1u;

        const std::uint32_t currentIndex = self->field_01a8;
        if (currentIndex != 0)
            self->field_01a8 = currentIndex - 1u;

        MenuEula_ComputeWriteCursorForEndPointer(
            &self->m_wtext0, nullptr, &erasedIterator,
            CGuiEdit_GetWideCharacterAddress(self, nullptr, originalIndex));
        AdjustTextRangeLeft(self, nullptr, 0);
    }
    else
    {
        const std::uint32_t currentIndex = self->field_01a8;
        if (currentIndex < visibleStart)
            self->field_01ac = visibleStart - 1u;
        if (currentIndex != 0)
            self->field_01a8 = currentIndex - 1u;

        MenuEula_ComputeWriteCursorForEndPointer(
            &self->m_wtext0, nullptr, &erasedIterator,
            CGuiEdit_GetWideCharacterAddress(self, nullptr, originalIndex));
        AdjustTextRangeRight(
            self, nullptr, static_cast<char>(originalIndex == 0));
    }
}

HOOK(0x721C10, CGuiEdit_DeleteCharacter, "auto");
