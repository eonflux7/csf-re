#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CanConfigureEffect, 0x007319C0, (void* self, void* edx, int effect));
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static hook::Tail __fastcall C_00867078_ForwardConfigureEffectTail(void* self, const uint32_t* /*args*/)
{
  int configurator = CanConfigureEffect(self, nullptr, 0);
  if (configurator != 0) {
    void* obj = reinterpret_cast<void*>(configurator);
    return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0x0C / 4]);
  }
  const float zeroValue = g_fZero;
  (void)zeroValue;
  return hook::tail_ret(0u);
}

HOOK_TAIL(0x00731AF0, C_00867078_ForwardConfigureEffectTail, "auto");
