#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());

// Calls the no-op scene loader, then tail-jumps to slot 0x9C of the
// vtable of the object stored in this->field_0308. The caller's stack
// arguments and return address are forwarded unchanged by hook::tail_jump.
static hook::Tail __fastcall C_0083dbb0_RunNoOpThenTailCallSubtask(void* self, const uint32_t* args) {
    auto* task = static_cast<C_0083dbb0*>(self);
    SceneLoader_NoOp();
    void* obj = reinterpret_cast<void*>(task->field_0308);
    return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0x9c / 4]);
}

HOOK_TAIL(0x004D6F20, C_0083dbb0_RunNoOpThenTailCallSubtask, "ai_autonomous");
