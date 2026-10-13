#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callee at 0x6C5C90: lazily fills the object's field at +0x90. Result unused by the caller.
CSF_FUNC(char, __fastcall, S_006d5c90_LazyInitTextureObject, 0x006D5C90, (void* self, void* edx));

// Returns the cached texture object at +0x90, initializing it first if it is zero.
static int __fastcall Water_GetTextureObject(S_006d5c90* self, void* /*edx*/)
{
    if (self->field_0090 == 0)
    {
        S_006d5c90_LazyInitTextureObject(self, nullptr);
    }
    return static_cast<int>(self->field_0090);
}

HOOK(0x006D5FB0, Water_GetTextureObject, "auto");
