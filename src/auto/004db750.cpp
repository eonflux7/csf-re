#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Forwards the call to virtual slot 22 (offset 0x58) of the object held at
// child->field_0010, passing the child and a2 as stack arguments. Returns 0
// when the child pointer at a1+0x0C is null. Plain function: no `this`.
static int __stdcall ForwardToChildAiStateVirtual22(int a1, int a2)
{
    int childTask = CSF_AT(int, reinterpret_cast<void*>(a1), 0x0C);
    if (childTask)
    {
        void* childTaskObj = CSF_AT(void*, reinterpret_cast<void*>(childTask), 0x10);
        void** childTaskVtbl = CSF_AT(void**, childTaskObj, 0);
        auto slotFn = reinterpret_cast<int(__fastcall*)(void*, void*, int, int)>(childTaskVtbl[22]);
        return slotFn(childTaskObj, nullptr, childTask, a2);
    }
    return childTask;
}

HOOK(0x004DB750, ForwardToChildAiStateVirtual22, "auto");
