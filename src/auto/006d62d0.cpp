#include "types/auto_classes.h"
#include "hook.h"

// Sets byte flag at offset 0xD8 to 1.
static void __fastcall C_00861120_SetEnabledFlag(C_00861120* self, void* /*edx*/)
{
    self->field_00d8 = 1;
}

HOOK(0x006D62D0, C_00861120_SetEnabledFlag, "auto");
