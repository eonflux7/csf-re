#include <cstdint>
#include "hook.h"

// Sets the byte flag at offset 0x7D of the object to 1.
// Original: mov byte ptr [ecx+0x7D], 1 ; ret 4  (__thiscall, 1 stack arg)
static void __fastcall C_528de0_SetFlag(void* self, void* /*edx*/, int /*a2*/)
{
    reinterpret_cast<uint8_t*>(self)[0x7d] = 1;
}

HOOK(0x528DE0, C_528de0_SetFlag, "auto");
