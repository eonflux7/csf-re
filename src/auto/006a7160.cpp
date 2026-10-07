#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Reads the object's virtual table and forwards a call to the method at
// vtable byte offset 0x44 (slot 17), passing 0 as its single argument,
// returning that method's result unchanged.
static int __fastcall InvokeVirtualSlot44WithZero(void* self, void* /*edx*/)
{
    void** vtbl = CSF_AT(void**, self, 0);
    return ((int(__fastcall*)(void*, void*, int))vtbl[0x44 / 4])(self, nullptr, 0);
}

HOOK(0x006A7160, InvokeVirtualSlot44WithZero, "auto");
