#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee at 0x0070CC60: __thiscall-style, self in ecx, edx unused.
CSF_FUNC(int, __fastcall, WaterFx_QueryInstance, 0x0070CC60, (void* self, void* edx));

// Original: push ebp; mov ebp,esp; mov ecx,[ebp+8]; call 0x70cc60; pop ebp; ret 4
// __stdcall with one stack argument, which the callee pops (ret 4).
static int __stdcall WaterFx_ForwardToStateCall(int a1)
{
    void* self = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(a1)));
    return WaterFx_QueryInstance(self, nullptr);
}

HOOK(0x709A70, WaterFx_ForwardToStateCall, "auto");
