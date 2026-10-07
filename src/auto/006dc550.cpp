#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// char __cdecl RpAnisotGetMaxSupportedMaxAnisotropy()
CSF_FUNC(signed char, __cdecl, RpAnisotGetMaxSupportedMaxAnisotropy, 0x566520, ());

// Returns true when the maximum anisotropy level supported by the renderer
// is strictly greater than zero (signed 8-bit comparison).
static bool __fastcall C_00861460_IsMaxAnisotropySupported(C_00861460* /*self*/, void* /*edx*/)
{
    const signed char maxAnisotropy = RpAnisotGetMaxSupportedMaxAnisotropy();
    return maxAnisotropy > 0;
}

HOOK(0x006DC550, C_00861460_IsMaxAnisotropySupported, "auto");
