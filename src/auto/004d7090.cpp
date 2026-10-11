#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

// Reads the pointer at this+8, then the object at +0x310 of that pointer.
// The original forwards to that object's vtable slot 0x90 (36) with the
// caller's stack arguments and return address untouched.
static hook::Tail __fastcall ForwardTailToSubobjectVirtual(void* self, const uint32_t* /*args*/) {
    void* owner = CSF_AT(void*, self, 0x08);
    void* targetObject = CSF_AT(void*, owner, 0x310);
    return hook::tail_jump(targetObject, (*reinterpret_cast<void***>(targetObject))[0x90 / 4]);
}

HOOK_TAIL(0x4D7090, ForwardTailToSubobjectVirtual, "ai_autonomous");
