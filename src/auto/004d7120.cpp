#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

// Reads the pointer at this+8, then the object at +0x30C of that pointer.
// The original forwards to that object's vtable slot 0x84 (33) with the
// caller's stack arguments and return address untouched.
static hook::Tail __fastcall ForwardTailToAutonomoSubobjectVirtual(void* self, const uint32_t* /*args*/) {
    void* owner = CSF_AT(void*, self, 0x08);
    void* targetObject = CSF_AT(void*, owner, 0x30C);
    return hook::tail_jump(targetObject, (*reinterpret_cast<void***>(targetObject))[0x84 / 4]);
}

HOOK_TAIL(0x4D7120, ForwardTailToAutonomoSubobjectVirtual, "ai_autonomous");
