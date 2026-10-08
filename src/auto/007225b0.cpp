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
static __attribute__((optnone)) void __fastcall CGuiEdit_MoveTextRangeRight(
    CGuiEdit* self, void* /*edx*/)
{
    const std::uint32_t textLength =
        CSF_AT(std::uint32_t, self, 0x164);
    if (textLength == 0)
        return;

    const std::uint32_t nextPosition = self->field_01b0 + 1u;
    if (nextPosition >= textLength)
        return;

    self->m_caretIndex = nextPosition;
    self->field_01ac = nextPosition;

    AdjustTextRangeRight(self, nullptr, 0);

    self->m_caretIndex = self->field_01b0;
    AdjustTextRangeLeft(self, nullptr, 0);
}

HOOK(0x007225B0, CGuiEdit_MoveTextRangeRight, "auto");
