#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x00495150 (7 bytes) — __thiscall, ret 0
// Increments the 32-bit counter at this->0x3E4 by one.
static void __fastcall IncrementCounterAt3E4(void* self, void* /*edx*/)
{
  CSF_AT(uint32_t, self, 0x3E4) += 1;
}

HOOK(0x495150, IncrementCounterAt3E4, "auto");
