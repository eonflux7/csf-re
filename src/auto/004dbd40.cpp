#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// 0x004DBD40 (7 bytes): sets the object's vtable pointer to the C_0082d410 vtable.
// Original: mov dword ptr [ecx], 0x82D410 ; ret
static void __fastcall C_0082d410_SetVtable(C_0082d410* self, void* /*edx*/)
{
    self->vtbl = reinterpret_cast<void**>(0x0082D410);
}

HOOK(0x4DBD40, C_0082d410_SetVtable, "auto");
