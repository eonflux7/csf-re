#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// 0x0048E700
// Reads the object's own vtable, calls the virtual method at vtable offset 0x28
// (slot 10) to obtain a nested object, then calls that nested object's virtual
// method at vtable offset 0x3C (slot 15) with the constant argument 1, returning
// the nested call's result.
static int __fastcall ForwardToNestedObjectMethod(void* self, void* /*edx*/)
{
    void** selfVtbl = CSF_AT(void**, self, 0);
    void* nested = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        selfVtbl[0x28 / 4])(self, nullptr);

    void** nestedVtbl = CSF_AT(void**, nested, 0);
    return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
        nestedVtbl[0x3C / 4])(nested, nullptr, 1);
}

HOOK(0x48E700, ForwardToNestedObjectMethod, "auto");
