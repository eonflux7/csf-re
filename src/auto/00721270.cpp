#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "hook.h"

// Returns a pointer to the character/data buffer of a small-buffer-optimized
// container living at this+0x154. If the capacity field at this+0x168 is less
// than 8, the data is stored inline (address this+0x154); otherwise the pointer
// stored at this+0x154 is used. (unsigned comparison, jb)
static void* __fastcall GetContainerDataPtr(void* self, void* /*edx*/)
{
  if (CSF_AT(uint32_t, self, 0x168) < 8u)
    return static_cast<uint8_t*>(self) + 0x154;
  return CSF_AT(void*, self, 0x154);
}

HOOK(0x721270, GetContainerDataPtr, "auto");
