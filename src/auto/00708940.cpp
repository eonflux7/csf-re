#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Vtable of C_008634c8 (the object's vtable pointer is set to this address).

// Constructor stub: installs the vtable pointer and returns `this`.
static C_008634c8* __fastcall C_008634c8_Constructor(C_008634c8* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x008634C8);
    return self;
}

HOOK(0x00708940, C_008634c8_Constructor, "auto");
