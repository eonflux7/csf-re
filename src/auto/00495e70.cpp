#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "hook.h"

// Returns whether the 32-bit field at offset 0x400 of the object is non-zero.
// Original:
//   mov edx, [ecx+0x400]
//   xor eax, eax
//   cmp eax, edx      ; unsigned compare 0 vs field
//   sbb eax, eax
//   neg eax           ; eax = (field != 0) ? 1 : 0
static int __fastcall IsField400NonZero(void* self, void* /*edx*/)
{
    return CSF_AT(uint32_t, self, 0x400) != 0;
}

HOOK(0x495E70, IsField400NonZero, "auto");
