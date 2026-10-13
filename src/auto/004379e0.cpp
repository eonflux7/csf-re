#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the address of the id-up slot of a CGuiControl. If the counter at +0x84
// is below 0x10 (unsigned), the address of the inline slot at +0x70 is returned;
// otherwise the pointer stored at +0x70 is returned.
static uint32_t* __fastcall CGuiControl_GetIdUp(CGuiControl* self, void* /*edx*/)
{
  if (CSF_AT(uint32_t, self, 0x84) < 0x10u)
    return &CSF_AT(uint32_t, self, 0x70);
  return reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x70));
}

HOOK(0x4379E0, CGuiControl_GetIdUp, "auto");
