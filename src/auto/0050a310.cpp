#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x0050A310: gets the global singleton (0x506BD0, GetGlobalValue_8B056C), then tail-jumps to a
// virtual method of one of its sub-objects. The indirect jmp forwards the caller's stack arguments untouched
// (target signature unknown), so this stays a naked thunk; eax is the getter's result either way.
static const uintptr_t global_getter = CSF_ADDR_FN(0x00506BD0);

static __declspec(naked) void CallGlobalObjectVtable9()
{
    __asm {
        mov eax, global_getter
        call eax
        mov ecx, dword ptr [eax + 0x1c]
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x24]
    }
}

HOOK(0x50A310, CallGlobalObjectVtable9, "auto");
