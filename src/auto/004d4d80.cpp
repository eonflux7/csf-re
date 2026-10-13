#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Forwards the call to the vtable slot at +0xA0 of the object reached through
// self->+0x08 -> +0x14 -> +0x30. The caller's arguments are passed through unchanged.
static hook::Tail __fastcall AiGiveObject_ForwardToLinkedObjectSlot40(void* self, const uint32_t* /*args*/) {
    uint32_t linkedObjectRef = CSF_AT(uint32_t, self, 0x08);
    uint32_t ownerRef = CSF_AT(uint32_t, reinterpret_cast<void*>(linkedObjectRef), 0x14);
    void* obj = reinterpret_cast<void*>(CSF_AT(uint32_t, reinterpret_cast<void*>(ownerRef), 0x30));
    return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0xA0 / 4]);
}

HOOK_TAIL(0x4D4D80, AiGiveObject_ForwardToLinkedObjectSlot40, "ai_give_object");
