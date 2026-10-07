#include "types/auto_classes.h"
#include "hook.h"

// Forwards the call to the object's virtual method at vtable byte offset 0xE4 (slot 57).
static int __fastcall C_00864288_ForwardToVirtualSlot57(C_00864288* self, void* /*edx*/)
{
    return ((int(__fastcall*)(void*, void*))self->vtbl[0xe4 / 4])(self, nullptr);
}

HOOK(0x00437080, C_00864288_ForwardToVirtualSlot57, "auto");
