#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// The original is a pure tail-call: it loads the object pointer stored at
// self+0x90, fetches that object's vtable, and jumps to the virtual method
// at vtable slot 5 (byte offset 0x14). The callee is a __thiscall with no
// stack arguments, so it returns a plain `ret` (0 stack bytes popped).
// We forward the callee's return value unchanged.
static int __fastcall InvokeMember90VirtualSlot5(void* self, void* /*edx*/)
{
    void* target = CSF_AT(void*, self, 0x90);
    void** vtbl = CSF_AT(void**, target, 0);
    return ((int(__fastcall*)(void*, void*))vtbl[5])(target, nullptr);
}

HOOK(0x51F7A0, InvokeMember90VirtualSlot5, "auto");
