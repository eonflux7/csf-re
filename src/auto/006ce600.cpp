#include <cstdint>
#include "hook.h"
#include "globals.h"

// The original pushes its [ebp-4] slot (never written) as the 4th argument to
// 0x6CE480. The forwarded value is garbage, so 0 is passed here.
CSF_FUNC(uint32_t*, __cdecl, sub_6CE480, 0x006CE480, (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4));

static uint32_t* __cdecl CollisionShapes_VectorAppendForward(uint32_t* a1, uint32_t* a2, uint32_t* a3)
{
  return sub_6CE480(a1, a2, a3, 0);
}

HOOK(0x006CE600, CollisionShapes_VectorAppendForward, "auto");
