#include <cstdint>
#include "globals.h"
#include "hook.h"

// Binds the original callee at 0x752520 (__fastcall, self in ecx, edx unused).
CSF_FUNC(void, __fastcall, sub_752520, 0x00752520, (void* self, void* edx));

// Forwards the second stdcall argument as `this` to 0x752520.
// Original: push ebp / mov ebp,esp / mov ecx,[ebp+0Ch] / call 0x752520 / pop ebp / ret 14h
static int __stdcall NetControlDdl_ForwardSecondArgToHandler(int a1, int a2, int a3, int a4, int a5) {
    (void)a1;
    (void)a3;
    (void)a4;
    (void)a5;
    sub_752520(reinterpret_cast<void*>(a2), nullptr);
    return 0;
}

HOOK(0x0074EAC0, NetControlDdl_ForwardSecondArgToHandler, "auto");
