#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee at 0x74B210: __thiscall-style method, ecx = this, edx unused. Its result is not used by this stub.
CSF_FUNC(void, __fastcall, NetCogibleDdl_DispatchSelf, 0x0074B210, (void* self, void* edx));

// Stub: forwards its second stack argument as `this` to the dispatcher. Five stdcall args, callee pops 20 bytes.
static int __stdcall NetCogibleDdl_ForwardSecondArgToDispatcher(int /*a1*/, int a2, int /*a3*/, int /*a4*/, int /*a5*/)
{
    void* self = reinterpret_cast<void*>(static_cast<uintptr_t>(a2));
    NetCogibleDdl_DispatchSelf(self, nullptr);
    return 0;
}

HOOK(0x00742A80, NetCogibleDdl_ForwardSecondArgToDispatcher, "auto");
