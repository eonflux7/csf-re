#include <cstdint>
#include "globals.h"
#include "hook.h"

// The original function is a pure tail-jump thunk: it loads ecx with the address of a
// singleton object (0x8BABFC) and then jmps into the __thiscall method sub_68F650,
// forwarding the single float argument. sub_68F650 pops 4 stack bytes (ret 4).
CSF_FUNC(int, __fastcall, Sub_68F650, 0x68F650, (void* self, void* edx, float a1));

// 0x0073C660: forwards its float argument to the global handler method sub_68F650,
// binding ecx to the singleton object at 0x8BABFC. Reproduced with the callee's
// exact __thiscall signature (written as __fastcall), so it pops the 4-byte argument.
static int __fastcall ForwardFloatToGlobalHandler(void* /*self*/, void* /*edx*/, float a1)
{
    return Sub_68F650(reinterpret_cast<void*>(static_cast<uintptr_t>(0x8BABFC)), nullptr, a1);
}

HOOK(0x73C660, ForwardFloatToGlobalHandler, "auto");
