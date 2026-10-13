#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original global at 0x008BB400, cleared on every construction.
CSF_GLOBAL(int, g_008bb400, 0x008BB400);

// __thiscall constructor for C_0085ebf4: installs the class vtable at offset 0,
// clears g_008bb400, and returns the object's address (the vtable slot pointer).
static uint32_t* __fastcall C_0085ebf4_Construct(C_0085ebf4* self, void* /*edx*/) {
    self->vtbl = CSF_VTBL(0x0085EBF4);
    g_008bb400 = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0069FBA0, C_0085ebf4_Construct, "auto");
