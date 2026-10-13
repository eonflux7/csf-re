#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee at 0x7531D0: __fastcall, self in ecx, no stack args (ret 0).
CSF_FUNC(void, __fastcall, sub_7531D0, 0x007531D0, (void* self, void* edx));

// 0x74B840: __stdcall with 5 args (ret 0x14). Forwards its second argument
// as `this` to the callee at 0x7531D0 and returns.
static int __stdcall NetGhostDDL_ForwardSecondArg(int a1, int a2, int a3, int a4, int a5) {
    (void)a1;
    (void)a3;
    (void)a4;
    (void)a5;
    sub_7531D0(reinterpret_cast<void*>(static_cast<uintptr_t>(a2)), nullptr);
    return 0;
}

HOOK(0x0074B840, NetGhostDDL_ForwardSecondArg, "auto");
