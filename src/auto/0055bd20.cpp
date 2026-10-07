#include "types/auto_classes.h"
#include "hook.h"

// Reads the byte flag at +0x04; if it is non-zero, clears it to 0 and returns the
// previous (non-zero) value. The upper bytes of eax are left untouched by the
// original, so the result is a plain byte (`char`).
static char __fastcall C_00845368_ConsumeFlag(C_00845368* self, void* /*edx*/)
{
    char flag = static_cast<char>(self->field_0004);
    if (flag)
        self->field_0004 = 0;
    return flag;
}

HOOK(0x0055BD20, C_00845368_ConsumeFlag, "auto");
