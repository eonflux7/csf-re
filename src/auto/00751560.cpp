#include <cstdint>
#include "globals.h"
#include "hook.h"

// Fallback in case CSF_AT is not provided by the included headers
// (raw byte access at a fixed offset).
#ifndef CSF_AT
#define CSF_AT(T, p, o) (*reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(p) + (o)))
#endif

// 0x00751560 (6 bytes): sets the byte flag at offset +0x24 on the object and
// returns true. Original: mov al, 1 / mov byte ptr [ecx+0x24], al / ret
// (__thiscall, no stack arguments -> ret 0).
static char __fastcall C_751560_SetFlag(void* self, void* /*edx*/)
{
    CSF_AT(uint8_t, self, 0x24) = 1;
    return 1;
}

HOOK(0x751560, C_751560_SetFlag, "auto");
