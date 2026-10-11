#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Forwards a virtual call (slot 0x80 / 4 = 32) on the object stored at
// self->field_0008->field_030C. The original is a tail jump, so the
// caller's stack arguments are passed through untouched via hook::tail_jump.
static hook::Tail __fastcall AiAutonomous_ForwardToSubobjectSlot32(void* self, const uint32_t* args)
{
    // eax = self->field_0008 (a pointer)
    void* owner = CSF_AT(void*, self, 0x08);
    // ecx = owner->field_030C (the object that receives the virtual call)
    void* obj = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x30C));
    // edx = obj->vtbl; jump to vtbl[0x80 / 4]
    return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0x80 / 4]);
}

HOOK_TAIL(0x004D7100, AiAutonomous_ForwardToSubobjectSlot32, "ai_autonomous");
