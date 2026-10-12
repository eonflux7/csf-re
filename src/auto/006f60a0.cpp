#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void, __fastcall, ClearLightsBlock, 0x006D2390, (void* self, void* edx));

// Flushes the pending entry list (last to first, via the class's slot-4 virtual),
// resets the count, and if the dirty flag was set, clears both light blocks.
static void __fastcall C_00862874_FlushAndClearLights(C_00862874* self, void* /*edx*/)
{
  // The original references the 0x006D2390 vtable address at its call/jmp sites.
  (void)CSF_VTBL(0x006D2390);

  for (int i = static_cast<int>(self->field_0010) - 1; i >= 0; --i)
    self->vSlot4(reinterpret_cast<void**>(self->field_0008 + 4u * static_cast<uint32_t>(i)));

  uint8_t dirtyFlag = self->field_0005;
  self->field_0010 = 0;
  if (dirtyFlag) {
    self->field_0005 = 0;
    ClearLightsBlock(&self->field_0058, nullptr);
    ClearLightsBlock(&self->field_0018, nullptr);
  }
}

HOOK(0x006F60A0, C_00862874_FlushAndClearLights, "auto");
