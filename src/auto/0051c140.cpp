#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

// The original stores the immediate vtable address 0x00840EA4 into the
// object's first field (there is no global variable bound at that address).
static void __fastcall C_00840ea4_Construct(C_00840ea4* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00840EA4);
}

HOOK(0x0051C140, C_00840ea4_Construct, "auto");
