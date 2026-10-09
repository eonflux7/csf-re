#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

static __declspec(naked) int __fastcall SceneLoader_ForwardLoadingState(
    void* state, void* /*edx*/, int a1, int a2)
{
    __asm {
        mov eax, ecx
        mov edx, dword ptr [eax]
        jmp dword ptr [edx + 0xc]
    }
}

static int __stdcall SceneLoader_UpdateLoadingState(int a1, int a2)
{
    void* globalValue = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
    void* state = reinterpret_cast<void*>(
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, globalValue, 0)[0x70 / 4])(
                globalValue, nullptr));

    if (!state)
        return 0;

    return SceneLoader_ForwardLoadingState(state, nullptr, a1, a2);
}

HOOK(0x00521F40, SceneLoader_UpdateLoadingState, "auto");
