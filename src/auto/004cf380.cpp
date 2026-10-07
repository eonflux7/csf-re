#include <cstdint>
#include "globals.h"
#include "types/auto_classes.h"
#include "hook.h"

// 0x004CF380 (ret 0) — __thiscall comparison of two dwords in the object.
// Asm: mov eax,[ecx+0x48] / cmp eax,[ecx+0x44] / sbb eax,eax / neg eax
// => returns CF of (dword@0x48 - dword@0x44), i.e. 1 iff dword@0x48 < dword@0x44 (UNSIGNED).
static int __fastcall IsField48LessThanField44(void* self, void* /*edx*/)
{
    return CSF_AT(uint32_t, self, 0x48) < CSF_AT(uint32_t, self, 0x44);
}

HOOK(0x4CF380, IsField48LessThanField44, "auto");
