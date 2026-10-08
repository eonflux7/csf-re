#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AdjustTextRangeRight, 0x00721900,
         (CGuiEdit* self, void* edx, char forceStart));
CSF_FUNC(int, __fastcall, CGuiEdit_MoveIndicesToTextEnd, 0x007224E0,
         (CGuiEdit* self, void* edx));

// The original helpers implicitly consume EBX. Avoid allocating locals there.
static __attribute__((optnone)) void __fastcall
CGuiEdit_MoveIndicesLeft(CGuiEdit* self, void* /*edx*/)
{
    const std::uint32_t textLength = CSF_AT(std::uint32_t, self, 0x164);
    if (textLength == 0)
        return;

    void* text = reinterpret_cast<void*>(self->m_pText);
    const int atStart =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, text, 0)[0x88 / 4])(text, nullptr);

    const std::uint32_t visibleStart = self->field_01ac;
    if (visibleStart == 0)
    {
        const std::uint32_t caretIndex = self->m_caretIndex;
        if (caretIndex != 0)
        {
            self->m_caretIndex = caretIndex - 1;
            AdjustTextRangeRight(self, nullptr, 0);
        }
        else if (static_cast<std::uint8_t>(atStart) != 0)
        {
            CGuiEdit_MoveIndicesToTextEnd(self, nullptr);
        }
        else
        {
            AdjustTextRangeRight(self, nullptr, 1);
        }
        return;
    }

    const std::uint32_t endPlusOne = self->field_01b0 + 1u;
    const std::uint32_t caretIndex = self->m_caretIndex;
    if (endPlusOne == textLength)
    {
        if (caretIndex < visibleStart)
            self->field_01ac = visibleStart - 1;
        if (caretIndex != 0)
            self->m_caretIndex = caretIndex - 1;
        AdjustTextRangeRight(self, nullptr, 0);
        return;
    }

    char forceStart = 0;
    if (caretIndex < visibleStart)
    {
        forceStart = (caretIndex == 0) ? 1 : 0;
        self->field_01ac = visibleStart - 1;
    }
    if (caretIndex != 0)
        self->m_caretIndex = caretIndex - 1;
    AdjustTextRangeRight(self, nullptr, forceStart);
}

HOOK(0x00722600, CGuiEdit_MoveIndicesLeft, "auto");
