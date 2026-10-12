#include <cstdint>
#include "globals.h"
#include "hook.h"

// Ends the current scene context. Asks the context object (self+0x14) whether it
// can finish (vtable slot 3). If that returns zero, forwards the call to the same
// object's vtable slot 2 via a tail jump.
static hook::Tail __fastcall SceneLoader_EndContext(void* self, const uint32_t* /*args*/)
{
    // First query: ecx = self->contextObject (+0x14), call its vtable[0x0C / 4].
    void* contextObject = CSF_AT(void*, self, 0x14);
    int result = ((int(__fastcall*)(void*, void*))(CSF_AT(void**, contextObject, 0)[0x0C / 4]))(contextObject, nullptr);

    // test al, al: only the low byte decides.
    if ((result & 0xFF) != 0) {
        // Original returns eax unchanged (plain ret, no stack args popped).
        return hook::tail_ret(result);
    }

    // Original reloads self->contextObject and tail-jumps through vtable[0x08 / 4].
    void* contextObjectAgain = CSF_AT(void*, self, 0x14);
    return hook::tail_jump(contextObjectAgain, (*reinterpret_cast<void***>(contextObjectAgain))[0x08 / 4]);
}

HOOK_TAIL(0x0069AEF0, SceneLoader_EndContext, "game");
