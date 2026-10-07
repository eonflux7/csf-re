#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x0072A920 — reached by tail jump from 0x72E7C0 with ecx = this and its
// second argument supplied in ebx (per the original's usercall prototype).
CSF_FUNC(char, __fastcall, CGuiControl_NotifySelectionChanged, 0x72A920, (void* self, void* edx));

// Virtual member of the control reached through vtable byte offset +0x158
// (slot 0x158/4): __thiscall taking one int on the stack (callee pops 4).
using CGuiControl_SetStateFn = void(__fastcall*)(void* self, void* edx, int value);

// Calls the control's vtable slot +0x158 with 0, then forwards to 0x72A920.
static char __fastcall CGuiRadioGroup_DeselectAndNotify(CGuiRadioGroup* self, void* /*edx*/)
{
    reinterpret_cast<CGuiControl_SetStateFn>(CSF_AT(void**, self, 0)[0x158 / 4])(self, nullptr, 0);
    return CGuiControl_NotifySelectionChanged(self, nullptr);
}

HOOK(0x72E7C0, CGuiRadioGroup_DeselectAndNotify, "auto");
