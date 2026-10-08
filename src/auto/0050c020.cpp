#include "hook.h"

// 0x0050C020 — CMenuScreenFader
//
// Reads the screen-fader overlay object pointer stored at +0x34. If it is
// null, returns 0 (eax already zero). Otherwise it tail-jumps into the
// overlay's virtual method at vtable byte offset +8 (slot 2), forwarding
// ecx = overlay and the caller's (empty) stack argument list unchanged.
//
// Written as a naked thunk because the original ends in an indirect tail jump
// (`jmp dword ptr [eax + 8]`) whose callee signature is unknown; the callee's
// return value is passed straight through to our caller.
static __declspec(naked) int CMenuScreenFader_ShowOverlay()
{
    __asm
    {
        mov     eax, dword ptr [ecx + 0x34]
        test    eax, eax
        je      no_overlay
        mov     ecx, eax
        mov     eax, dword ptr [ecx]
        jmp     dword ptr [eax + 8]
    no_overlay:
        ret
    }
}

HOOK(0x0050C020, CMenuScreenFader_ShowOverlay, "auto");
