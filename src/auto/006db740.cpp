#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x006DB740 (6 bytes):
//   mov eax, 0x5c
//   ret
// Returns the constant 0x5C (92). Does not touch `this`, so it is safe to
// model as a thiscall-style member that only returns the value.
static int __fastcall CRenderEngine_GetSize(CRenderEngine* /*self*/, void* /*edx*/)
{
    return 92;
}

HOOK(0x6DB740, CRenderEngine_GetSize, "auto");
