#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns a pointer to the idDown data word of a CGuiControl.
// If the reserve counter at +0xA0 is below 0x10 (unsigned), the address of the
// inline field at +0x8C is returned; otherwise the pointer stored at +0x8C is returned.
static uint32_t* __fastcall CGuiControl_GetIdDown(CGuiControl* self, void* /*edx*/)
{
  if (CSF_AT(uint32_t, self, 0xA0) < 0x10u)
    return &CSF_AT(uint32_t, self, 0x8C);
  return reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x8C));
}

HOOK(0x437A00, CGuiControl_GetIdDown, "auto");
