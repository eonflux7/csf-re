#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x00728B60 (46 bytes)
//
// Gate deciding whether this object may activate:
//   - virtual slot vtbl+0x18 must return non-zero,
//   - byte flag at +0x40 must be set,
//   - pointers at +0x50 and +0x54 must both be non-null.
// Only then the original tail-jumps to the virtual at vtbl+0x90. Every failing
// path returns the value eax held at that point of the original code: 0 after
// the three pointer tests, and the vtbl+0x18 result with its low byte replaced
// by the (zero) +0x40 flag after the flag test.

// Naked thunk replacing the original's `mov edx,[esi]; mov ecx,esi; pop esi;
// jmp dword ptr [edx+0x90]`. Entered with ecx == this (__fastcall's first
// parameter), it recreates the original register state at the jump
// (ecx = this, edx = vtable, eax = *(this+0x54)) and performs the indirect
// jump instead of an indirect call through the vtable slot.
static __declspec(naked) char __fastcall C_00865a10_ForwardToVtbl90(C_00865a10* /*self*/, void* /*edx*/)
{
    __asm
    {
        mov eax, dword ptr [ecx + 0x54]
        mov edx, dword ptr [ecx]
        jmp dword ptr [edx + 0x90]
    }
}

static char __fastcall C_00865a10_TryActivateIfEnabled(C_00865a10* self, void* /*edx*/)
{
    // original: mov eax,[esi]; call dword ptr [eax+0x18]; test eax,eax; je out
    int gateResult = reinterpret_cast<int(__fastcall*)(C_00865a10*, void*)>(self->vtbl[6])(self, nullptr);
    if (!gateResult)
        return 0;

    // original: mov al, byte ptr [esi+0x40]; test al,al; je out
    if (!self->field_0040)
        return static_cast<char>(gateResult & ~0xFF); // al = 0, upper bytes of the call result kept

    // original: mov eax, dword ptr [esi+0x50]; test eax,eax; je out
    if (!self->field_0050)
        return 0;

    // original: mov eax, dword ptr [esi+0x54]; test eax,eax; je out
    if (!self->field_0054)
        return 0;

    // original: mov edx,[esi]; mov ecx,esi; pop esi; jmp dword ptr [edx+0x90]
    return C_00865a10_ForwardToVtbl90(self, nullptr);
}

HOOK(0x728B60, C_00865a10_TryActivateIfEnabled, "auto");
