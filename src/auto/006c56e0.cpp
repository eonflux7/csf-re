#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns the object size in bytes (0x148 = 328).
static int __fastcall C_008608e8_GetSize(C_008608e8* /*self*/, void* /*edx*/)
{
    return 328;
}

HOOK(0x006C56E0, C_008608e8_GetSize, "auto");
