#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// 0x004762F0 (12 bytes):
//   mov eax, 1
//   mov [ecx+0x50], eax
//   mov [ecx+0x4c], eax
//   ret
// Sets two dword flags on the object to 1 and returns 1.
//
// Fields 0x4c and 0x50 are not declared in the known auto-classes, so they are
// accessed raw by offset.
static int __fastcall SetEnableFlags(void* self, void* /*edx*/)
{
    CSF_AT(uint32_t, self, 0x50) = 1;
    CSF_AT(uint32_t, self, 0x4c) = 1;
    return 1;
}

HOOK(0x004762F0, SetEnableFlags, "auto");
