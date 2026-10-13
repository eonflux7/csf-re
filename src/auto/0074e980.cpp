#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __fastcall, NetControlDdl_ProcessObject, 0x00752C40, (void* self, void* edx));

// Forwards the second stack argument as `this` to the object-processing routine.
static int __stdcall NetControlDdl_ForwardSecondArgCall(int a1, int a2, int a3, int a4, int a5)
{
    (void)a1;
    (void)a3;
    (void)a4;
    (void)a5;
    NetControlDdl_ProcessObject(reinterpret_cast<void*>(static_cast<intptr_t>(a2)), nullptr);
    return 0;
}

HOOK(0x0074E980, NetControlDdl_ForwardSecondArgCall, "auto");
