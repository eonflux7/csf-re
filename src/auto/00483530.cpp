#include <cstdint>

#include "globals.h"
#include "hook.h"

// Thunk: forwards to the virtual method at vtable byte offset 0x134 (slot 0x134/4)
// with stack arguments (2, 0), passing `self` as the this pointer, and returns
// the callee's result unchanged.
static int __fastcall CallVirtualAt0x134With2And0(void* self, void* /*edx*/)
{
    void** vtbl = *reinterpret_cast<void***>(self);
    auto method = reinterpret_cast<int(__fastcall*)(void*, void*, int, int)>(vtbl[0x134 / 4]);
    return method(self, nullptr, 2, 0);
}

HOOK(0x483530, CallVirtualAt0x134With2And0, "auto");
