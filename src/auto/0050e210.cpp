#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// __thiscall BOOL sub_50E210(_DWORD* this)
//   mov edx, [ecx+0xF0]
//   xor eax, eax
//   test edx, edx
//   setne al
//   ret
// Returns non-zero when the dword field at offset +0xF0 is non-zero.
static int __fastcall Object_IsFieldF0NonZero(void* self, void* /*edx*/)
{
    return CSF_AT(uint32_t, self, 0xF0) != 0;
}

HOOK(0x50E210, Object_IsFieldF0NonZero, "auto");
