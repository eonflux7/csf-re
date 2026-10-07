#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "globals.h"

// The original forwards (a1, a2, a3) to 0x723580 plus a 4th stack argument read from an
// uninitialized 4-byte local ([ebp-4], which the prologue's `push ecx` reserves). The callee is
// __cdecl (ret 0), so this caller cleans the four argument slots itself.
CSF_FUNC(uint32_t*, __cdecl, Sub_723580, 0x723580, (uint32_t* a1, uint32_t* a2, uint32_t* a3, uint32_t a4));

static uint32_t* __cdecl ForwardTo723580(uint32_t* a1, uint32_t* a2, uint32_t* a3)
{
  // Deliberately uninitialized: reproduces the read of the untouched [ebp-4] slot.
  volatile uint32_t stackLocal;
  return Sub_723580(a1, a2, a3, stackLocal);
}

HOOK(0x7237A0, ForwardTo723580, "auto");
