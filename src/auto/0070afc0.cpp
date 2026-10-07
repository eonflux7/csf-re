#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns the size (in bytes) of a C_0086315c instance.
static int __fastcall C_0086315c_GetSize(C_0086315c* /*self*/, void* /*edx*/)
{
    return 0x30;
}

HOOK(0x0070AFC0, C_0086315c_GetSize, "auto");
