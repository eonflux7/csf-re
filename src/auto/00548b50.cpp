#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Reads field_002c (pointer to the currently selected audio device object).
// If present, queries that object through vtable slot 14 (0x38/4) and stores
// the low byte of the result into field_0068 (Switch_SB_XFI flag).
static void __fastcall CMenuSoundOptionsPC_RefreshXFiSwitchState(CMenuSoundOptionsPC* self, void* /*edx*/)
{
  uint32_t obj = self->field_002c;
  if (obj)
  {
    void* device = reinterpret_cast<void*>(obj);
    void** vtbl = CSF_AT(void**, device, 0);
    int result = ((int(__fastcall*)(void*, void*))vtbl[14])(device, nullptr);
    self->field_0068 = static_cast<uint8_t>(result);
  }
}

HOOK(0x00548B50, CMenuSoundOptionsPC_RefreshXFiSwitchState, "auto");
