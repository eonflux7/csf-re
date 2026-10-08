#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// 0x00542D50 — __thiscall, ret 4 (one stack argument, unused by the body).
// If the pending state (this+0x2D8) is 1, latch the applied flag (this+0x2C8 = 1),
// clear the secondary value (this+0x2CC = 0) and move the state to 2.
// Always returns 1.
static int __fastcall CMenuControls_ApplyPendingState(void* self, void* /*edx*/, int /*a2*/)
{
  int result = 1;

  if (CSF_AT(int, self, 0x2D8) == 1)
  {
    CSF_AT(int, self, 0x2C8) = 1;
    CSF_AT(int, self, 0x2CC) = 0;
    CSF_AT(int, self, 0x2D8) = 2;
  }

  return result;
}

HOOK(0x542D50, CMenuControls_ApplyPendingState, "auto");
