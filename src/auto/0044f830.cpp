#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound by address from the packet (copied exactly).
CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(unsigned int, __cdecl, SoundManager_ApplyGlobalFloatValue, 0x00525250, ());

// Sets the control's stored float value to -1.0f (bit pattern 0xBF800000), then
// forwards to the sound manager using the value returned by Get_008b0678.
static int __fastcall CGuiControl_ClearCurrentIndexState(CGuiControl* self, void* /*edx*/)
{
    self->field_0120 = 0xBF800000u;
    Get_008b0678();
    return static_cast<int>(SoundManager_ApplyGlobalFloatValue());
}

HOOK(0x0044F830, CGuiControl_ClearCurrentIndexState, "auto");
