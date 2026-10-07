#include <cstdint>
#include "hook.h"
#include "core/memory.h"

// 0x00495880 (7 bytes) — __thiscall getter:
//   mov al, byte ptr [ecx + 0x3a5]
//   ret
// Returns the byte field at offset 0x3A5 of the object.
static char __fastcall C_00495880_GetField3A5(uint8_t* self, void* /*edx*/)
{
    return static_cast<char>(CSF_AT(uint8_t, self, 0x3a5));
}

HOOK(0x495880, C_00495880_GetField3A5, "auto");
