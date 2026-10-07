#include "types/auto_classes.h"
#include "hook.h"

// Returns field_0018 (offset 0x18) of the object. The argument is accepted
// and ignored; the callee pops 4 bytes of stack arguments (ret 4).
static int __fastcall C_00863430_GetField0018(C_00863430* self, void* /*edx*/, int /*a2*/)
{
    return static_cast<int>(self->field_0018);
}

HOOK(0x00708340, C_00863430_GetField0018, "auto");
