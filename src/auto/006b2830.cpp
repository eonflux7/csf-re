#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Initializes a C_00860170 object by installing its class vtable pointer.
// Original: mov dword ptr [ecx], 0x860170 ; ret
static void __fastcall C_00860170_Construct(C_00860170* self, void* /*edx*/)
{
    self->vtbl = reinterpret_cast<void**>(0x00860170);
}

HOOK(0x6B2830, C_00860170_Construct, "auto");
