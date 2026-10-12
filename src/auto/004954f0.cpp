#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char, __fastcall, ToggleTimer_SetState, 0x004A1260, (uint8_t* self, void* , char a2));

// Hud_SetSuspended: forwards to ToggleTimer_SetState on the embedded sub-object at +0x344.
// The original is a tail jump, so the callee's result in al is returned unchanged.
static int __fastcall HudSetSuspended(uint8_t* self, void* /*edx*/, char a1)
{
    return ToggleTimer_SetState(self + 0x344, nullptr, a1);
}

HOOK(0x004954F0, HudSetSuspended, "auto");
