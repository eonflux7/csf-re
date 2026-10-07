#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// sub_718C00(this, 1.0f)  -- __thiscall, pops 4 stack bytes
CSF_FUNC(int, __fastcall, CGuiSwitch_Sub_718C00, 0x718C00, (int* self, void* edx, int a2));

// CGuiControl_ResetInput(this)  -- __thiscall, pops 0 stack bytes
CSF_FUNC(int, __fastcall, CGuiControl_ResetInput, 0x717F70, (CGuiControl* self, void* edx));

static int __fastcall CGuiSwitch_Reset(CGuiSwitch* self, void* /*edx*/)
{
    reinterpret_cast<int*>(self)[83] = 0;             // [esi+0x14c] = 0
    reinterpret_cast<int*>(self)[95] = 0x3F800000;    // [esi+0x17c] = 1.0f
    CGuiSwitch_Sub_718C00(reinterpret_cast<int*>(self), nullptr, 0x3F800000);
    return CGuiControl_ResetInput(self, nullptr);     // tail jmp -> 0x717F70
}

HOOK(0x72FE00, CGuiSwitch_Reset, "auto");
