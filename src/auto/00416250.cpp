#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082deb8_ReleaseChildBase_41E110, 0x0041E110, (C_0082e520* self, void*));

// Resets C_0082e090 to its own vtable, releases the child base, then re-points
// the secondary slot at +0xAC and the primary vtable to C_0082d410.
static int __fastcall C_0082e090_ResetVtablesAndReleaseChild(C_0082e090* self, void* /*edx*/)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082E090);
    int result = C_0082deb8_ReleaseChildBase_41E110(self, nullptr);
    CSF_AT(void*, self, 0xAC) = CSF_VTBL(0x0082D410);
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x00416250, C_0082e090_ResetVtablesAndReleaseChild, "auto");
