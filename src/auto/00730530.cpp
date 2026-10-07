#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x007301C0 - switch-specific per-frame update (animation/text-colour handling).
// Signature per packet: void __thiscall(float* this, float dt); ret 4.
CSF_FUNC(void, __fastcall, CGuiSwitch_UpdateVisualState, 0x7301C0,
         (CGuiSwitch* self, void* /*edx*/, float dt));

// 0x00719530 - CGuiControl base update (returns a char result, ret 4).
CSF_FUNC(char, __fastcall, CGuiControl_Update, 0x719530,
         (CGuiControl* self, void* /*edx*/, float dt));

// 0x00730530 - CGuiSwitch::Update
// Runs the switch-specific update and then the base control update,
// forwarding the base result unchanged.
static char __fastcall CGuiSwitch_Update(CGuiSwitch* self, void* /*edx*/, float dt)
{
    CGuiSwitch_UpdateVisualState(self, nullptr, dt);
    return CGuiControl_Update(self, nullptr, dt);
}

HOOK(0x730530, CGuiSwitch_Update, "auto");
