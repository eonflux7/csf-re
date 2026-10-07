#include "types/auto_classes.h"
#include "hook.h"

// C_00842150::ResetSomeState  (0x005384C0)
// Clears fields at +0x30, +0x34 (dwords) and +0x50 (byte) and returns 0.
static int __fastcall C_00842150_ResetState(C_00842150* self, void* /*edx*/)
{
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0050 = 0;
    return 0;
}

HOOK(0x005384C0, C_00842150_ResetState, "auto");
