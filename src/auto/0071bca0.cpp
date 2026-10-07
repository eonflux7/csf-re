#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "globals.h"

// Target 0x71BCA0 forwards its three incoming arguments to 0x71B670, adding a
// fourth stack argument read from an uninitialized 4-byte local ([ebp-4],
// reserved by the prologue's `push ecx`). The callee is __cdecl, so this caller
// cleans the four argument slots (16 bytes) itself.
CSF_FUNC(void*, __cdecl, Sub_71B670, 0x71B670, (void* a1, void* a2, void* a3, void* a4));

static void* __cdecl ForwardArgsTo71B670(void* a1, void* a2, void* a3)
{
  // Reproduces the read of the untouched [ebp-4] slot (leftover value of ecx).
  volatile uintptr_t uninitializedSlot;
  return Sub_71B670(a1, a2, a3, reinterpret_cast<void*>(uninitializedSlot));
}

HOOK(0x71BCA0, ForwardArgsTo71B670, "auto");
