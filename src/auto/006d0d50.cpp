#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Tail-forwards to the virtual slot at vtable offset 0x34 (index 13) of the
// object stored in this->field_002c, passing that object as `this`.
static int __fastcall C_00860e68_InvokeInnerVirtual34(C_00860e68* self, void* /*edx*/)
{
    void* inner = reinterpret_cast<void*>(self->field_002c);
    void** innerVtbl = CSF_AT(void**, inner, 0);
    return ((int(__fastcall*)(void*, void*))innerVtbl[13])(inner, nullptr);
}

HOOK(0x6D0D50, C_00860e68_InvokeInnerVirtual34, "auto");
