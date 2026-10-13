#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Forwards the call to virtual slot 30 (offset 0x78) of the object held at
// child->field_0010, passing the child and a2 as stack arguments. Returns 0
// when the child pointer at a1+0x0C is null. Plain function: no `this`.
static int __stdcall ForwardToChildAiStateSlot30(int a1, int a2)
{
    int childState = CSF_AT(int, reinterpret_cast<void*>(a1), 0x0C);
    if (childState)
    {
        void* childStateObj = CSF_AT(void*, reinterpret_cast<void*>(childState), 0x10);
        void** childStateVtbl = CSF_AT(void**, childStateObj, 0);
        auto slotFn = reinterpret_cast<int(__fastcall*)(void*, void*, int, int)>(childStateVtbl[30]);
        return slotFn(childStateObj, nullptr, childState, a2);
    }
    return childState;
}

HOOK(0x004DB850, ForwardToChildAiStateSlot30, "auto");
