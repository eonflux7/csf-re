#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

// Vtable the constructor installs into the object at offset 0.
static void** const g_Vtbl_C_0083e050 = CSF_VTBL(0x0083E050);

// Constructor: stores the class vtable into the object and returns the object.
static TAiState* __fastcall TAiState_Constructor(TAiState* self, void* /*edx*/)
{
    self->vtbl = g_Vtbl_C_0083e050;
    return self;
}

HOOK(0x004DBF70, TAiState_Constructor, "auto");
