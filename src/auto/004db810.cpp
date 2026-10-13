#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Forwards the call to virtual slot 28 (offset 0x70) of the object held at
// child->field_0010, passing that object as `this` and the child and a2 as
// stack arguments. Returns 0 when the child pointer at a1+0x0C is null.
// Plain function: no `this` parameter; callee pops 8 bytes (ret 8).
static int __stdcall ForwardToChildTaskVirtualSlot28(int a1, int a2)
{
    int childTask = CSF_AT(int, reinterpret_cast<void*>(a1), 0x0C);
    if (childTask)
    {
        void* childTaskObj = CSF_AT(void*, reinterpret_cast<void*>(childTask), 0x10);
        void** childTaskVtbl = CSF_AT(void**, childTaskObj, 0);
        auto slotFn = reinterpret_cast<int(__fastcall*)(void*, void*, int, int)>(childTaskVtbl[28]);
        return slotFn(childTaskObj, nullptr, childTask, a2);
    }
    return childTask;
}

HOOK(0x004DB810, ForwardToChildTaskVirtualSlot28, "auto");
