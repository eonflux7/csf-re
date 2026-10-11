#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t*, __cdecl, sub_6D9E00, 0x006D9E00, (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4));

static uint32_t* __cdecl ProfileSaveGame_ForwardTo6D9E00(uint32_t* a1, uint32_t* a2, uint32_t* a3)
{
  // The original forwards its unwritten [ebp-4] local as the 4th argument; pass 0 for it.
  int unusedLocalArg = 0;
  return sub_6D9E00(a1, a2, a3, unusedLocalArg);
}

HOOK(0x00514910, ProfileSaveGame_ForwardTo6D9E00, "auto");
