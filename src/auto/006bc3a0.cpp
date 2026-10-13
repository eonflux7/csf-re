#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __cdecl, GameSleepMilliseconds, 0x0069E450, (uint32_t dwMilliseconds));

// Waits until the pending-operation counter at +0xC0 drains, then releases the
// stream buffer at +0xB4 and clears the four bookkeeping dwords (+0xB4..+0xC0).
// Returns the Mem_Free result when a buffer was released, otherwise 0.
static int __fastcall C_008605d8_FlushStreamBuffers(int* self, void* /*edx*/)
{
  while (CSF_AT(int, self, 0xC0) != 0) {
    GameSleepMilliseconds(10);
  }

  int result = 0;
  void* streamBuffer = CSF_AT(void*, self, 0xB4);
  if (streamBuffer != nullptr) {
    result = Mem_Free(streamBuffer);
    CSF_AT(void*, self, 0xB4) = nullptr;
    CSF_AT(int, self, 0xB8) = 0;
    CSF_AT(int, self, 0xBC) = 0;
    CSF_AT(int, self, 0xC0) = 0;
  }
  return result;
}

HOOK(0x006BC3A0, C_008605d8_FlushStreamBuffers, "auto");
