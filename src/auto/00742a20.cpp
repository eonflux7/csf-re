#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee at 0x751570: __fastcall, takes `this` in ecx, returns nothing per its bound signature.
CSF_FUNC(void, __fastcall, NetCogibleDdl_ProcessDdlObject, 0x00751570, (void* self, void* edx));

// Original: push ebp / mov ebp,esp / mov ecx,[ebp+0xC] / call 0x751570 / pop ebp / ret 0x14
// Forwards its second stack argument as `this` to the callee and returns whatever eax holds afterwards.
static int __stdcall NetCogibleDdl_ForwardSecondArg(int a1, int a2, int a3, int a4, int a5)
{
    (void)a1;
    (void)a3;
    (void)a4;
    (void)a5;
    NetCogibleDdl_ProcessDdlObject(reinterpret_cast<void*>(static_cast<std::intptr_t>(a2)), nullptr);
    return 0;
}

HOOK(0x00742A20, NetCogibleDdl_ForwardSecondArg, "auto");
