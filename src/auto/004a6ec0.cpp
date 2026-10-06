#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Returns the constant 6 (likely a fixed count/size reported by this class).
static int __fastcall C_0082fdb8_GetSize(C_0082fdb8* self, void* /*edx*/)
{
    (void)self;
    return 6;
}

HOOK(0x4A6EC0, C_0082fdb8_GetSize, "auto");
