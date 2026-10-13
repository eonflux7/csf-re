#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __fastcall, sub_752980, 0x00752980, (void* self, void* edx));

// Original: int __stdcall (int a1..a5), ret 0x14. Forwards its second stack argument
// to 0x752980 as `this` (ecx). The callee's result is not propagated; see notes.
static int __stdcall NetControl_ForwardSecondArgToHandler(int a1, int a2, int a3, int a4, int a5) {
    (void)a1;
    (void)a3;
    (void)a4;
    (void)a5;
    sub_752980(reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(a2))), nullptr);
    return 0;
}

HOOK(0x0074EBB0, NetControl_ForwardSecondArgToHandler, "auto");
