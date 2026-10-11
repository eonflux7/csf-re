#include <cstdint>
#include "hook.h"
#include "globals.h"

CSF_FUNC(uint32_t*, __cdecl, sub_755C50, 0x00755C50, (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4));

// The original forwards its three arguments to 0x755C50 and passes the never-written
// [ebp-4] slot (reserved by the prologue's `push ecx`) as a fourth argument. That value
// is uninitialized garbage in the original, so it is passed as 0 here.
static uint32_t* __cdecl GameConfig_ForwardThreeArgsTo755C50(uint32_t* a1, uint32_t* a2, uint32_t* a3)
{
  const int uninitializedLocal = 0;
  return sub_755C50(a1, a2, a3, uninitializedLocal);
}

HOOK(0x755DE0, GameConfig_ForwardThreeArgsTo755C50, "auto");
