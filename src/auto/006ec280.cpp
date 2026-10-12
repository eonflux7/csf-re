#include "hook.h"
#include "types/auto_classes.h"

static char __fastcall C_0086200c_ResetQueryState(C_0086200c* self, void* /*edx*/)
{
    self->field_000c = 0;
    self->field_0018 = 1;
    self->field_0019 = 0;
    self->field_0038 = 1;
    return 1;
}

HOOK(0x006EC280, C_0086200c_ResetQueryState, "auto");
