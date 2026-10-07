#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Sets the vtable pointer of a C_0085e65c object (constructor / vtable install).
static void __fastcall C_0085e65c_InitVtable(C_0085e65c* self, void* /*edx*/)
{
    self->vtbl = reinterpret_cast<void**>(0x0085E65C);
}

HOOK(0x006923A0, C_0085e65c_InitVtable, "auto");
