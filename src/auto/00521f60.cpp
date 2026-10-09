#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

static __declspec(naked) char __fastcall AudioSoundManager_ForwardActiveSound(
    void* /*self*/, void* /*edx*/) {
    __asm {
        mov edx, dword ptr [ecx]
        jmp dword ptr [edx + 0x10]
    }
}

static char __cdecl AudioSoundManager_QueryActiveSound() {
    void* manager = reinterpret_cast<void*>(
        static_cast<uintptr_t>(GetGlobalValue_70DDE0()));

    int sound = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, manager, 0)[0x70 / 4])(manager, nullptr);

    if (!sound)
        return 0;

    return AudioSoundManager_ForwardActiveSound(
        reinterpret_cast<void*>(static_cast<uintptr_t>(sound)), nullptr);
}

HOOK(0x00521F60, AudioSoundManager_QueryActiveSound, "auto");
