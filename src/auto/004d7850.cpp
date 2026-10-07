#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns the constant 14. Likely a virtual method of C_0083b770 (vtable 0x0083B770)
// that reports some kind / type id. The body ignores `this`; the callee pops no
// stack bytes (ret 0).
static int __fastcall C_0083b770_GetConst14(C_0083b770* self, void* /*edx*/)
{
    (void)self;
    return 14;
}

HOOK(0x4D7850, C_0083b770_GetConst14, "auto");
