#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t*, __cdecl, MenuVideo_CopyRecordsBackward, 0x00549B50, (uint32_t* rangeStart, uint32_t* rangeEnd, uint32_t* destEnd));

static uint32_t* __cdecl GfxEnvironment_ForwardToCopyRecordsBackward(uint32_t* a1, uint32_t* a2, uint32_t* a3)
{
  return MenuVideo_CopyRecordsBackward(a1, a2, a3);
}

HOOK(0x6BCEF0, GfxEnvironment_ForwardToCopyRecordsBackward, "auto");
