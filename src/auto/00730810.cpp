#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x007143D0 — base helper called by the original; __thiscall(this, int) -> int, ret 4.
CSF_FUNC(int, __fastcall, CGuiListBoxBase_Sub7143D0, 0x7143D0, (void* self, void* /*edx*/, int value));

// int __thiscall sub_730810(CGuiTextureListBox *this, int a2)
// Forwards the argument to the base helper 0x7143D0, then stores it as a raw dword
// into the CGuiTextureListBox field at +0x208 (declared uint32_t field_0208),
// and returns the base helper's result.
static int __fastcall CGuiTextureListBox_SetField0208(CGuiTextureListBox* self, void* /*edx*/, int value)
{
    int result = CGuiListBoxBase_Sub7143D0(self, nullptr, value);
    self->field_0208 = static_cast<uint32_t>(value);
    return result;
}

HOOK(0x730810, CGuiTextureListBox_SetField0208, "auto");
