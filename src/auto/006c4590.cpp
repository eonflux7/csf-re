#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// C_00860894 method: returns the object size in bytes (0x14 == sizeof(C_00860894)).
// Original body: mov eax, 0x14 ; ret  (this/edx are never touched).
static int __fastcall C_00860894_GetSize(C_00860894* /*self*/, void* /*edx*/)
{
    return 20;
}

HOOK(0x006C4590, C_00860894_GetSize, "auto");
