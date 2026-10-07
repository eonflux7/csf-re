#include <cstdint>
#include "hook.h"

// NEGATIVE CONTROL (bug class from the lanes trial, found via x32dbg): `add ecx, 0x30` takes the ADDRESS of the
// sub-object embedded at +0x30; this version LOADS a pointer from +0x30 instead (tail jump kept, so only the
// embedded-object check can catch it).
static __declspec(naked) void C_00860288_DispatchSubobjectVirtual70()
{
    __asm {
        mov ecx, dword ptr [ecx + 0x30]
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x70]
    }
}

HOOK(0x6B2F40, C_00860288_DispatchSubobjectVirtual70, "auto");
