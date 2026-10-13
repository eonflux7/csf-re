#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Stack layout in the original: a3 is pushed first, then a2, so the callee sees
// a2 as its arg0 (at [esp+4]) and a3 as its second stack argument (at [esp+8]).
// a1 is loaded into ecx as the callee's this.
CSF_FUNC(char, __fastcall, NetPlayerDdl_SendBaseFields, 0x007A3A90, (void* self, void* edx, void* arg0, int a3));

static char __stdcall NetChatDdl_ForwardToSendBaseFields(int a1, int a2, int a3)
{
    void* self = reinterpret_cast<void*>(static_cast<uintptr_t>(a1));
    void* arg0 = reinterpret_cast<void*>(static_cast<uintptr_t>(a2));
    return NetPlayerDdl_SendBaseFields(self, nullptr, arg0, a3);
}

HOOK(0x00750F30, NetChatDdl_ForwardToSendBaseFields, "auto");
