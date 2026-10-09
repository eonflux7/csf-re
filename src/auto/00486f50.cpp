#include "hook.h"
#include "globals.h"
#include "core/memory.h"

static __declspec(naked) int __fastcall
CharActionFsm_ResolveSceneHandleFallback(void*, void*) {
    __asm {
        mov edx, dword ptr [ecx]
        jmp dword ptr [edx + 0xdc]
    }
}

static int __fastcall
CharActionFsm_ResolveSceneHandle(void* self, void* /*edx*/) {
    const int sceneIndex = CSF_AT(int, self, 0x28);
    int handle = 0;

    if (sceneIndex != -1)
        handle = CSF_AT(int*, self, 0x18)[sceneIndex];

    if (handle != 0)
        return handle;

    [[clang::musttail]]
    return CharActionFsm_ResolveSceneHandleFallback(self, nullptr);
}

HOOK(0x00486F50, CharActionFsm_ResolveSceneHandle, "auto");
