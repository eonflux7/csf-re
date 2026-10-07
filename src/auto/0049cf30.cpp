#include "types/auto_classes.h"
#include "hook.h"

// Sets the byte flag at offset 0x198 (base C_00834050::field_0198) to 1.
static void __fastcall C_00834198_SetField0198(C_00834198* self, void* /*edx*/)
{
    self->field_0198 = 1;
}

HOOK(0x49CF30, C_00834198_SetField0198, "auto");
