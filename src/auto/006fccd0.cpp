#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the size of the CLightObject object in bytes (0x84 == 132),
// matching sizeof(CLightObject). Likely a virtual size/allocator accessor.
static int __fastcall CLightObject_GetObjectSize(CLightObject* /*self*/, void* /*edx*/)
{
    return 132;
}

HOOK(0x006FCCD0, CLightObject_GetObjectSize, "auto");
