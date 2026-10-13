#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Forwards the update to virtual slot 11 (offset 0x2C) of the object held at
// child->field_0010, passing the child and a2 as stack arguments. Returns the
// child pointer (0) when the child at a1+0x0C is null. Plain function: no `this`.
static int __stdcall AiState_ForwardUpdateToChildTask(int a1, int a2)
{
    int childTask = CSF_AT(int, reinterpret_cast<void*>(a1), 0x0C);
    if (childTask)
    {
        void* childTaskObj = CSF_AT(void*, reinterpret_cast<void*>(childTask), 0x10);
        void** childTaskVtbl = CSF_AT(void**, childTaskObj, 0);
        auto slotFn = reinterpret_cast<int(__fastcall*)(void*, void*, int, int)>(childTaskVtbl[11]);
        return slotFn(childTaskObj, nullptr, childTask, a2);
    }
    return childTask;
}

HOOK(0x004DBD50, AiState_ForwardUpdateToChildTask, "auto");
