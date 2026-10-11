#include <cstdint>
#include "hook.h"

// CRT identity helper: returns its first stack argument unchanged.
// cdecl, so the caller pops the argument (ret 0).
static int __cdecl GfxObjectProps_IdentityReturnArg(int value) {
    return value;
}

HOOK(0x6B8500, GfxObjectProps_IdentityReturnArg, "auto");
