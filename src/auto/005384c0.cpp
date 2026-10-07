#include "types/auto_classes.h"
#include "hook.h"

// CMenuLoading::ResetSomeState  (0x005384C0)
// Clears fields at +0x30, +0x34 (dwords) and +0x50 (byte) and returns 0.
static int __fastcall CMenuLoading_ResetState(CMenuLoading* self, void* /*edx*/)
{
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0050 = 0;
    return 0;
}

HOOK(0x005384C0, CMenuLoading_ResetState, "auto");
