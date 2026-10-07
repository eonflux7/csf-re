#include <cstdint>
#include "globals.h"
#include "types/auto_classes.h"
#include "hook.h"

// Thin __thiscall forwarding thunk: takes the object stored in this->field_0004
// and tail-jumps to its virtual method at vtable offset 0x50 (slot 20),
// returning that method's result unchanged.
static int __fastcall C_008443c8_ForwardToInnerVtblSlot20(C_008443c8* self, void* /*edx*/)
{
    void* inner = reinterpret_cast<void*>(self->field_0004);
    return ((int (__fastcall*)(void*, void*))CSF_AT(void**, inner, 0)[20])(inner, nullptr);
}

HOOK(0x0054E4F0, C_008443c8_ForwardToInnerVtblSlot20, "auto");
