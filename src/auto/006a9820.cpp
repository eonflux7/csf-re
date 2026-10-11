#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, sub_6A7770, 0x006A7770, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, sub_6A8880, 0x006A8880, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, sub_6A90B0, 0x006A90B0, (void* self, void* edx, int a2));

// Dispatches on the phase stored at +0x24: 0 -> 90B0, 1 -> 8880, 4 -> 7770, others do nothing.
static void __fastcall S_006a7640_DispatchByPhase(S_006a7640* self, void* /*edx*/, int a2)
{
  uint32_t phase = self->field_0024;
  if (phase == 0) {
    sub_6A90B0(self, nullptr, a2);
  } else if (phase == 1) {
    sub_6A8880(self, nullptr, a2);
  } else if (phase == 4) {
    sub_6A7770(self, nullptr, a2);
  }
}

HOOK(0x006A9820, S_006a7640_DispatchByPhase, "auto");
