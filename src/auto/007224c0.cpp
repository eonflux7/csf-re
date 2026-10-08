#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Helper at 0x7215F0. Its real convention is: a1 in ecx, a2 in ebx, one stack
// char argument (callee pops 4 bytes). We bind it as __fastcall(self, edx, char):
// ecx and the single stack arg line up exactly. The helper reads a2 from ebx, and
// this wrapper never writes ebx (the original 0x7224C0 leaves ebx untouched too),
// so the caller's ebx is forwarded transparently.
CSF_FUNC(int, __fastcall, AdjustTextRangeLeft, 0x7215F0, (CGuiEdit* self, void* /*edx*/, char a3));

// 0x7224C0: if the edit control's character count (wstring size stored at +0x164)
// is nonzero, store (count - 1) into the two caret / anchor fields (+0x1A8, +0x1B0)
// and refresh the control through 0x7215F0, returning its result.
static int __fastcall CGuiEdit_HandleBackspace(CGuiEdit* self, void* /*edx*/)
{
    uint32_t charCount = CSF_AT(uint32_t, self, 0x164);
    if (charCount == 0)
        return 0;

    uint32_t lastIndex = charCount - 1;
    self->field_01a8 = lastIndex;
    self->field_01b0 = lastIndex;

    return AdjustTextRangeLeft(self, nullptr, 0);
}

HOOK(0x7224C0, CGuiEdit_HandleBackspace, "auto");
