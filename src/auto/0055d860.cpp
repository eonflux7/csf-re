#include <cstdint>
#include "globals.h"
#include "hook.h"

// C_00833908::SetOwnedContext (__thiscall, one stack arg, ret 4).
// Stores the context pointer at this+0x48.
static void __fastcall C_00833908_SetOwnedContext(void* self, void* /*edx*/, void* context)
{
    CSF_AT(void*, self, 0x48) = context;
}

HOOK(0x0055D860, C_00833908_SetOwnedContext, "auto");
