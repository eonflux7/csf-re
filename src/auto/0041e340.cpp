#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Current scene object (global at 0x8AFC90). Its field at +0x130 is the scene child whose
// virtual slot 7 (byte offset 0x1C) is the forwarding target.
CSF_GLOBAL(void*, g_pCurrentScene, 0x8AFC90);

// Forwards the caller's arguments and return address unchanged to
// (*g_pCurrentScene->child_0130)->vtbl[0x1C / 4], with the child in ecx.
static hook::Tail __fastcall AiTaskStates_ForwardToSceneChildSlot7(void* self, const uint32_t* args) {
    (void)self;
    (void)args;
    void* sceneChild = CSF_AT(void*, g_pCurrentScene, 0x130);
    return hook::tail_jump(sceneChild, (*reinterpret_cast<void***>(sceneChild))[0x1C / 4]);
}

HOOK_TAIL(0x41E340, AiTaskStates_ForwardToSceneChildSlot7, "ai_task_states");
