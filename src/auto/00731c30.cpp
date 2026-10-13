#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CanConfigureEffect, 0x007319C0, (void* self, void* edx, int effect));

// 0x00731C30: asks CanConfigureEffect(this, 4) for a handler object. If one exists,
// tail-jumps to its vtable slot 9 (+0x24). Otherwise returns 0.
static hook::Tail __fastcall C_00867078_TailToEffectHandler(void* self, const uint32_t* args) {
    (void)args;
    int handlerPtr = CanConfigureEffect(self, nullptr, 4);
    if (handlerPtr != 0) {
        void* handler = reinterpret_cast<void*>(static_cast<intptr_t>(handlerPtr));
        return hook::tail_jump(handler, (*reinterpret_cast<void***>(handler))[0x24 / 4]);
    }
    return hook::tail_ret(0);
}

HOOK_TAIL(0x731C30, C_00867078_TailToEffectHandler, "auto");
