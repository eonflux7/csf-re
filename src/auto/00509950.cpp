// Commandos: Strike Force (2006, Pyro Studios) — hook reimplementation
// Target: 0x00509950 (9 bytes) — __thiscall setter that stores 0 into the
// object's first dword, ignoring its (single, stack-passed) argument.
//
//   00509950  mov dword ptr [ecx], 0
//   00509956  ret 4

#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Stores 0 into the dword at offset 0 of the object and discards the argument.
static void __fastcall ClearFirstField(void* self, void* /*edx*/, int /*unusedValue*/)
{
    CSF_AT(uint32_t, self, 0) = 0;
}

HOOK(0x509950, ClearFirstField, "auto");
