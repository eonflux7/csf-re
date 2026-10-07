#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Forwards to the second virtual method (vtable slot 1) of the sub-object
// stored at this+0x90, passing that sub-object as the this-pointer.
static int __fastcall CallInnerObjectAt90_SecondVirtual(void* self, void* /*edx*/)
{
    void* innerObject = CSF_AT(void*, self, 0x90);
    void** innerVtbl = CSF_AT(void**, innerObject, 0);
    return ((int (__fastcall*)(void*, void*))innerVtbl[1])(innerObject, nullptr);
}

HOOK(0x50DDD0, CallInnerObjectAt90_SecondVirtual, "auto");
