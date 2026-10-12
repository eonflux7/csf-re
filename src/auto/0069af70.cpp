#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_ActiveSceneContext, 0x008BAF90);

CSF_FUNC(char, __fastcall, MissionScript_HasPendingState, 0x0069AF10, (void* self, void* edx));

// Original: if the active scene context exists and has a pending state, tail-jump
// to the context's vtable slot 1 (+4) with the context as `this`. Otherwise return.
static hook::Tail __fastcall SceneLoader_ServiceLoading(void* self, const uint32_t* args) {
    (void)self;
    (void)args;

    if (g_ActiveSceneContext == nullptr) {
        return hook::tail_ret(0);
    }

    if (!MissionScript_HasPendingState(g_ActiveSceneContext, nullptr)) {
        return hook::tail_ret(0);
    }

    // The original reloads the global before the indirect jump.
    void* obj = g_ActiveSceneContext;
    return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0x04 / 4]);
}

HOOK_TAIL(0x0069AF70, SceneLoader_ServiceLoading, "auto");
