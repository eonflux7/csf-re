#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Constant-return virtual method of C_00861460 (vtable 0x00861460).
// Original body: mov eax, 0x60 ; ret  -> always returns 96 (0x60).
static int __fastcall C_00861460_GetValue(C_00861460* self, void* /*edx*/)
{
    (void)self;
    return 96;
}

HOOK(0x6DC170, C_00861460_GetValue, "auto");
