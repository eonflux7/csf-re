#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Bind of the callee at 0x7319C0. ecx is not reloaded before the call, so it still
// holds the caller's `this` on entry; that is passed as `self`.
CSF_FUNC(int, __fastcall, CanConfigureEffect, 0x007319C0, (void* self, void* edx, int effect));

// Tail-forwarding path: if CanConfigureEffect(this, 4) returns a non-null object,
// jump through its vtable slot at +0x1C with that object in ecx. Otherwise return 0.
static hook::Tail __fastcall TailForwardToConfigurableEffect(void* self, const uint32_t* args)
{
    (void)args;

    int effectObject = CanConfigureEffect(self, nullptr, 4);
    if (effectObject != 0)
    {
        void* obj = reinterpret_cast<void*>(static_cast<uintptr_t>(effectObject));
        return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0x1C / 4]);
    }

    return hook::tail_ret(0);
}

HOOK_TAIL(0x00731C10, TailForwardToConfigurableEffect, "game");
