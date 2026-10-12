#include <cstdint>
#include "hook.h"
#include "globals.h"

// Callee at 0x6CE550: 4 stack args, __cdecl, returns the element pointer in eax.
CSF_FUNC(uint32_t*, __cdecl, CollisionShapes_VectorInsertHelper, 0x006CE550,
         (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4));

// Forwards its three arguments plus the never-written [ebp-4] local as the 4th argument.
static uint32_t* __cdecl CollisionShapes_ForwardToVectorInsertHelper(uint32_t* a1, uint32_t* a2, uint32_t* a3)
{
  // The original pushes the uninitialized [ebp-4] slot as the 4th argument. Per the
  // rules, an uninitialized value passed along is garbage, so pass 0.
  int uninitArg = 0;
  return CollisionShapes_VectorInsertHelper(a1, a2, a3, uninitArg);
}

HOOK(0x006CE670, CollisionShapes_ForwardToVectorInsertHelper, "auto");
