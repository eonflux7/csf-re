#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "globals.h"

// 0x547030 forwards (a1, a2, a3) to 0x546F10 plus a 4th argument taken from the
// uninitialized 4-byte stack slot reserved by the prologue (`push ecx` -> [ebp-4]).
// The callee is __cdecl (ret 0), so this caller cleans the 4 pushed argument slots.
CSF_FUNC(uint32_t*, __cdecl, Sub_546F10, 0x546F10,
         (uint32_t* a1, uint32_t* a2, uint32_t* a3, uint32_t* a4));

static uint32_t* __cdecl ForwardTo546F10(uint32_t* a1, uint32_t* a2, uint32_t* a3)
{
  // Intentionally uninitialized: reproduces the read of the untouched [ebp-4] slot.
  volatile uint32_t stackLocal;
  uint32_t rawLocal = stackLocal;
  return Sub_546F10(a1, a2, a3, reinterpret_cast<uint32_t*>(rawLocal));
}

HOOK(0x547030, ForwardTo546F10, "auto");
