#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Forwards the call to the virtual method at vtable slot 4 (byte offset 0x10)
// of the sub-object stored in this->field_0040. Tail call, no stack arguments.
static int __fastcall C_008410b0_InvokeSubobjectVirtual4(C_008410b0* self, void* /*edx*/)
{
    void* subobject = reinterpret_cast<void*>(self->field_0040);
    void** subobject_vtbl = *reinterpret_cast<void***>(subobject);
    return ((int(__fastcall*)(void*, void*))subobject_vtbl[4])(subobject, nullptr);
}

HOOK(0x526850, C_008410b0_InvokeSubobjectVirtual4, "auto");
