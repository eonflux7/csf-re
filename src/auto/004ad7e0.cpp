#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "core/memory.h"

// 0x004AD7E0 (__thiscall, 11 bytes, ret 0):
//   mov edx, [ecx+8] ; xor eax,eax ; test edx,edx ; setge al ; ret
// Returns 1 if the signed 32-bit value at object+8 is >= 0, otherwise 0.
static int32_t __fastcall IsField8NonNegative(void* self, void* /*edx*/)
{
    return CSF_AT(int32_t, self, 0x0008) >= 0;   // test/setge -> signed comparison
}

HOOK(0x004AD7E0, IsField8NonNegative, "auto");
