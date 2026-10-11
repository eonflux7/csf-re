#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Forwards a call through the object stored at +0x308 to its virtual slot 0xAC.
// Original: mov ecx,[ecx+0x308]; mov eax,[ecx]; jmp [eax+0xAC]
static hook::Tail __fastcall C_0083dbb0_TailForwardToInnerVirtual172(void* self, const uint32_t* args) {
    (void)args;
    C_0083dbb0* owner = reinterpret_cast<C_0083dbb0*>(self);
    void* innerObject = reinterpret_cast<void*>(owner->field_0308);
    return hook::tail_jump(innerObject, (*reinterpret_cast<void***>(innerObject))[0xAC / 4]);
}

HOOK_TAIL(0x4D7770, C_0083dbb0_TailForwardToInnerVirtual172, "ai_autonomous");
