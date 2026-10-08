#include <cstdint>
#include "hook.h"
#include "globals.h"

// 0x00690E60 int __cdecl Mem_FreeDebug(void* Block)
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x690E60, (void* Block));

// Destructor-like helper: frees the buffer stored at +0x04 and clears the
// three pointer slots at +0x04, +0x08 and +0x0C. Returns the freed buffer
// pointer (eax from the free call, or the original slot-4 value when null).
static int __cdecl MenuManager_FreeBufferAndClear(int* a1)
{
  int result = a1[1];
  if ( result )
    result = Mem_FreeDebug(reinterpret_cast<void*>(a1[1]));
  a1[1] = 0;
  a1[2] = 0;
  a1[3] = 0;
  return result;
}

HOOK(0x50C9C0, MenuManager_FreeBufferAndClear, "auto");
