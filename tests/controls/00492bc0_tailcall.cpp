#include <cstdint>
#include "globals.h"
#include "hook.h"

// Thunk: loads the member object stored at this+0x44 and tail-calls the
// virtual method at vtable offset 0x110 (slot 0x44) on that object.
static int __fastcall CallInnerObjectVirtualMethod110(void* self, void* /*edx*/)
{
    void* innerObject = CSF_AT(void*, self, 0x44);
    void** innerVtable = CSF_AT(void**, innerObject, 0x00);
    return ((int(__fastcall*)(void*, void*))innerVtable[0x110 / 4])(innerObject, nullptr);
}

HOOK(0x492BC0, CallInnerObjectVirtualMethod110, "auto");
