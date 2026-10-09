#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, MenuSystem_ToggleChildFlag, 0x0050D300, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

static int __cdecl AudioSoundManager_PrepareSceneLoadTarget()
{
    int state = GetGlobalState_50D190();
    MenuSystem_ToggleChildFlag(
        reinterpret_cast<void*>(static_cast<std::uintptr_t>(state)), nullptr);
    return GetGlobalValue_70DDE0();
}

static int (__cdecl* g_PrepareSceneLoadTarget)() =
    &AudioSoundManager_PrepareSceneLoadTarget;

static __declspec(naked) void __cdecl SceneLoader_CheckLoadResult()
{
    __asm {
        mov eax, dword ptr [g_PrepareSceneLoadTarget]
        call eax
        mov edx, dword ptr [eax]
        mov ecx, eax
        jmp dword ptr [edx + 0x18]
    }
}

HOOK(0x00521DC0, SceneLoader_CheckLoadResult, "auto");
