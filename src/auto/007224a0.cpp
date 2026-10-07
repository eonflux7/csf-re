#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// sub_721900: CGuiEdit helper (__thiscall-like). ecx = this, one stack char arg,
// callee pops 4 bytes. Its declared inputs are @<ecx> and @<ebx> (ebx is forwarded
// from our own caller unchanged); edx is not listed as an input.
CSF_FUNC(int, __fastcall, CGuiEdit_RefreshFromText, 0x721900, (CGuiEdit* self, void* edx, char flag));

// If the edit's text buffer (wstring at +0x150, its size word lives at +0x164) is
// non-empty, reset the two state words at +0x1A8/+0x1AC and refresh. Otherwise
// return 0 without doing anything.
static int __fastcall CGuiEdit_ResetScrollIfTextPresent(CGuiEdit* self, void* /*edx*/)
{
    uint32_t textSize = CSF_AT(uint32_t, self, 0x164);
    if (textSize == 0)
        return 0;

    self->field_01a8 = 0;
    self->field_01ac = 0;

    // edx carries the loaded size value in the original (register reuse);
    // it is not an input of the callee but we forward it verbatim.
    return CGuiEdit_RefreshFromText(self, reinterpret_cast<void*>(textSize), 1);
}

HOOK(0x7224A0, CGuiEdit_ResetScrollIfTextPresent, "auto");
