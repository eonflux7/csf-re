#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Returns the size (0x44 = 68) of a C_008621c8 instance.
static int __fastcall C_008621c8_GetSize(C_008621c8* /*self*/, void* /*edx*/)
{
    return 68;
}

HOOK(0x6EF3E0, C_008621c8_GetSize, "auto");
