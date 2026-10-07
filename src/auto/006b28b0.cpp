#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns the constant 0xD4 (212). No object state is read and no arguments
// are consumed; the original compiles down to `mov eax, 0xD4; ret`.
static int __fastcall C_00860288_GetValue212(C_00860288* self, void* /*edx*/)
{
    (void)self;
    return 212;
}

HOOK(0x006B28B0, C_00860288_GetValue212, "auto");
