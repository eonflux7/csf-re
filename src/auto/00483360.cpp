#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns TRUE (32-bit) when the dword stored at offset 0x10 of the object is 36 (0x24).
// Compiled form:
//   mov edx, [ecx+0x10]
//   xor eax, eax
//   cmp edx, 0x24
//   sete al
//   ret
static int __fastcall IsFieldAt10Equal36(void* self, void* /*edx*/)
{
    return CSF_AT(uint32_t, self, 0x10) == 36u;
}

HOOK(0x483360, IsFieldAt10Equal36, "auto");
