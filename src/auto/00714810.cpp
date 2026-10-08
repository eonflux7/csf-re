#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x714750: helper the original tail-invokes after the line spacing change; receives the
// previous value of field_01b8 (== +0x1B8) as its argument.
CSF_FUNC(int, __fastcall, CGuiListBoxBase_SetItemHeight, 0x714750,
         (CGuiListBoxBase* self, void* edx, int a2));

// 0x714810: sets the row line spacing (+0x170) and re-runs the layout helper with the
// old contents of +0x1B8, then returns the helper's result.
static int __fastcall CGuiListBoxBase_SetLineSpacing(CGuiListBoxBase* self, void* /*edx*/, int a2)
{
    const int previousValue = static_cast<int>(self->field_01b8);
    self->m_lineSpacing = static_cast<uint32_t>(a2);
    return CGuiListBoxBase_SetItemHeight(self, nullptr, previousValue);
}

HOOK(0x714810, CGuiListBoxBase_SetLineSpacing, "auto");
