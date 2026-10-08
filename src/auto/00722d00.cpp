#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x007143D0 — base-setter called by the original: __thiscall(this, int) -> int, ret 4.
CSF_FUNC(int, __fastcall, CGuiListBoxBase_SetSliderLinkedValue, 0x7143D0, (void* self, void* /*edx*/, int value));

// int __thiscall sub_722D00(int *this, int a2)
// Forwards to the base helper 0x7143D0 with the same argument, then stores the
// argument as a raw dword into the CGuiListBox field at +0x22C (declared as float),
// and returns the base helper's result.
static int __fastcall CGuiListBox_SetField022c(CGuiListBox* self, void* /*edx*/, int value)
{
    int result = CGuiListBoxBase_SetSliderLinkedValue(self, nullptr, value);
    *reinterpret_cast<uint32_t*>(&self->field_022c) = static_cast<uint32_t>(value);
    return result;
}

HOOK(0x722D00, CGuiListBox_SetField022c, "auto");
