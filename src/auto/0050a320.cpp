#include <cstdint>
#include "hook.h"

// 0x0050A320: gets the global singleton (0x506BD0, GetGlobalValue_8B056C), then tail-jumps to a
// virtual method of one of its sub-objects. The indirect jmp forwards the caller's stack arguments untouched
// (target signature unknown), so this stays a naked thunk; eax is the getter's result either way.
static __declspec(naked) void CallGlobalObjectVirtualSlot10()
{
    __asm {
        mov eax, 0x506bd0
        call eax
        mov ecx, dword ptr [eax + 0x1c]
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x28]
    }
}

HOOK(0x50A320, CallGlobalObjectVirtualSlot10, "auto");
