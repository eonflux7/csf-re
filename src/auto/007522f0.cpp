#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_00868b04_ResetSpawnMessage(C_00868b04* self, void* /*edx*/)
{
    self->field_0020 = 0;
    self->field_0030 = 0;
    self->field_002c = 0;
    self->field_0028 = 0;
    self->field_0004 = 1;
    return 1;
}

HOOK(0x007522F0, C_00868b04_ResetSpawnMessage, "auto");
