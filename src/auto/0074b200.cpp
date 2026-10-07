#include <cstdint>
#include "hook.h"

// 0x0074B200: sets the byte flag at offset 0x1 to 1 and returns 1 (in AL).
// __thiscall, no stack arguments (ret 0).
static char __fastcall C_74B200_SetFlagAndReturnTrue(uint8_t* self, void* /*edx*/)
{
    self[1] = 1;
    return 1;
}

HOOK(0x74B200, C_74B200_SetFlagAndReturnTrue, "auto");
