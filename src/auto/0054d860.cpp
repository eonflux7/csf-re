#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static C_008443c4* __fastcall C_008443c4_Construct(
    C_008443c4* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x008443C4);
    self->field_0004 = 1;
    return self;
}

HOOK(0x0054D860, C_008443c4_Construct, "auto");
