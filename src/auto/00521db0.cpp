#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

// Fetches the global sound object and tail-jumps to its vtable slot 0x48 (index 18).
// The original's push/mov/pop ebp frame is balanced around the call and leaves ebp and esp
// unchanged at the jmp, so the thunk omits it. Caller's stack args and registers are forwarded untouched.
static __declspec(naked) void AudioSound_ForwardToGlobalObjectSlot18() {
    __asm {
        mov eax, offset GetGlobalValue_70DDE0
        call eax
        mov edx, dword ptr [eax]
        mov ecx, eax
        jmp dword ptr [edx + 0x48]
    }
}

HOOK(0x521DB0, AudioSound_ForwardToGlobalObjectSlot18, "auto");
