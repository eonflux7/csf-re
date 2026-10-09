#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

// Fetches the global sound object and tail-jumps to its vtable slot 0x74 (index 29).
// The original's push/mov/pop ebp frame leaves ebp and esp unchanged at the jmp, so the
// thunk omits them. Caller's stack args and registers are forwarded untouched.
static __declspec(naked) void AudioSound_ForwardToGlobalObjectSlot29() {
    __asm {
        mov eax, offset GetGlobalValue_70DDE0
        call eax
        mov edx, dword ptr [eax]
        mov ecx, eax
        jmp dword ptr [edx + 0x74]
    }
}

HOOK(0x521D90, AudioSound_ForwardToGlobalObjectSlot29, "auto");
