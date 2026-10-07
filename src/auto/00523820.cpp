#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Calls the virtual method at vtable slot 0x2c/4 (= 11) on this object, then
// clears the byte flag at offset 0x3c, returning the virtual call's result.
static int __fastcall C_00840ff0_ClearFlagAfterVirtualCall(C_00840ff0* self, void* /*edx*/)
{
    int result = ((int(__fastcall*)(void*, void*))self->vtbl[11])(self, nullptr);
    self->field_003c = 0;
    return result;
}

HOOK(0x523820, C_00840ff0_ClearFlagAfterVirtualCall, "auto");
