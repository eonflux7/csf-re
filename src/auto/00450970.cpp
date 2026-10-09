#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(int, __cdecl, sub_525E70, 0x00525E70, ());

// Original: __thiscall with one stack arg, ret 4.
// If the stored state equals float -1.0 bits (0xBF800000), fetch the scene handle
// and run the cleanup routine, then store the argument and clear the state.
static int __fastcall CGuiControl_DispatchSceneValue(CGuiControl* self, void* /*edx*/, int value) {
  if (self->field_0120 == 0xBF800000u) {
    int sceneHandle = Get_008b0678();
    sub_525E70();
  }
  self->field_0124 = static_cast<uint32_t>(value);
  self->field_0120 = 0;
  return value;
}

HOOK(0x00450970, CGuiControl_DispatchSceneValue, "auto");
