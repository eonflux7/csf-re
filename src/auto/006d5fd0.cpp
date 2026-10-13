#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callee at 0x6D5C90: lazily creates the water render target stored at +0x94. Result unused by the caller.
CSF_FUNC(char, __fastcall, sub_6D5C90, 0x006D5C90, (void* self, void* edx));

// The original's call site references S_006d5c90's vtable address.
[[maybe_unused]] static const void* const kGfxWaterVtbl = CSF_VTBL(0x006D5C90);

// Returns the render target at +0x94, creating it first if it is still null.
static int __fastcall GfxWater_GetRenderTarget(S_006d5c90* self, void* /*edx*/)
{
    if (self->field_0094 == 0)
    {
        sub_6D5C90(self, nullptr);
    }
    return static_cast<int>(self->field_0094);
}

HOOK(0x006D5FD0, GfxWater_GetRenderTarget, "auto");
