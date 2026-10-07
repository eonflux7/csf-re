// 0x00417D70 - C_0082e128 method: set flag byte at offset 0x210 to 1.
// Original disassembly:
//   mov byte ptr [ecx + 0x210], 1
//   ret
// __thiscall, callee pops 0 bytes (no stack args).

#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

static void __fastcall C_0082e128_SetFlag0210(C_0082e128* self, void* /*edx*/)
{
    self->field_0210 = 1;
}

HOOK(0x417D70, C_0082e128_SetFlag0210, "auto");
