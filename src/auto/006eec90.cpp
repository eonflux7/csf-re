#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Reads the vtable pointer at +0x08 (the vtable of the sub-object embedded at +0x08),
// then calls its virtual method at vtable byte offset 0x68 (slot 26) with a single
// argument of 0, returning whatever that method returns.
static int __fastcall C_00862110_CallSubobjectVirtual68(C_00862110* self, void* /*edx*/)
{
    void** subobject_vtbl = reinterpret_cast<void**>(self->field_0008);
    void* subobject = reinterpret_cast<uint8_t*>(self) + 8;
    return ((int(__fastcall*)(void*, void*, int))subobject_vtbl[26])(subobject, nullptr, 0);
}

HOOK(0x006EEC90, C_00862110_CallSubobjectVirtual68, "auto");
