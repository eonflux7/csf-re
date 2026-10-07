#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Writes 0 to the byte field at offset 0xF4 of the object.
// Disassembly:
//   006c5a30  mov byte ptr [ecx + 0xf4], 0
//   006c5a37  ret
static void __fastcall ClearFlagF4(uint8_t* self, void* /*edx*/)
{
  CSF_AT(uint8_t, self, 0xF4) = 0;
}

HOOK(0x006C5A30, ClearFlagF4, "auto");
