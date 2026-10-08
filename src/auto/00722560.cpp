#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

// Both helpers additionally consume the incoming EBX implicitly.
CSF_FUNC(int, __fastcall, AdjustTextRangeLeft, 0x007215F0,
         (CGuiEdit* self, void* edx, char mode));
CSF_FUNC(int, __fastcall, AdjustTextRangeRight, 0x00721900,
         (CGuiEdit* self, void* edx, char mode));

// Keep this wrapper unoptimized so its simple operations do not allocate EBX.
static __attribute__((optnone)) int __fastcall CGuiEdit_MoveTextRangeLeft(
    CGuiEdit* self, void* /*edx*/)
{
    if (CSF_AT(std::uint32_t, self, 0x164) == 0)
        return 0;

    const std::uint32_t position = self->field_01ac;
    if (position == 0)
        return 0;

    const std::uint32_t previousPosition = position - 1u;
    self->m_caretIndex = previousPosition;
    self->field_01b0 = previousPosition;
    self->field_01ac = previousPosition;

    AdjustTextRangeLeft(self, nullptr, 0);

    self->m_caretIndex = self->field_01ac;
    return AdjustTextRangeRight(self, nullptr, 1);
}

HOOK(0x00722560, CGuiEdit_MoveTextRangeLeft, "auto");
