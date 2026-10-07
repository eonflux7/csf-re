#include <cstdint>
#include "globals.h"
#include "types/auto_classes.h"
#include "hook.h"

// 0x006D0D60
// Loads the object pointer stored at this+0x30 and tail-calls its virtual method
// at vtable byte offset 0x34 (slot 13), forwarding the (empty) argument list and
// returning its int result.
static int __fastcall C_00860e68_ForwardToInnerVirtualSlot13(C_00860e68* self, void* /*edx*/)
{
    void* inner = reinterpret_cast<void*>(self->field_0030);
    void** inner_vtbl = CSF_AT(void**, inner, 0);
    return ((int(__fastcall*)(void*, void*))inner_vtbl[13])(inner, nullptr);
}

HOOK(0x006D0D60, C_00860e68_ForwardToInnerVirtualSlot13, "auto");
