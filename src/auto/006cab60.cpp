#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callee at 0x6CA970: lazily fills the object's field at +0x10. Result unused by the caller.
CSF_FUNC(int, __fastcall, C_00860bb8_LazyInitField0010, 0x006CA970, (void* self, void* edx));

// Returns the cached value at +0x10, initializing it first if it is zero.
static int __fastcall C_00860bb8_GetOrInitField0010(C_00860bb8* self, void* /*edx*/)
{
    if (self->field_0010 == 0)
    {
        C_00860bb8_LazyInitField0010(self, nullptr);
    }
    return static_cast<int>(self->field_0010);
}

HOOK(0x006CAB60, C_00860bb8_GetOrInitField0010, "auto");
