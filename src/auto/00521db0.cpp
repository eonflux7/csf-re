#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

// Fetches the global sound object and tail-jumps to its vtable slot 0x48 (index 18) with the object as
// `this`; the caller's stack arguments are forwarded untouched.
static hook::Tail __fastcall AudioSound_ForwardToGlobalObjectSlot18(void* self, const uint32_t* args)
{
    (void)self;
    (void)args;
    void* manager = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
    return hook::tail_jump(manager, (*reinterpret_cast<void***>(manager))[0x48 / 4]);
}

HOOK_TAIL(0x521DB0, AudioSound_ForwardToGlobalObjectSlot18, "auto");
