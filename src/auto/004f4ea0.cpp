#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Reads this->field_0004 (an embedded provider object), asks it for its
// referenced object (vtable slot 4), then tail-jumps into that object's
// vtable slot 4 with the result as `this`.
static hook::Tail __fastcall ForwardProviderFrameState(void* self, const uint32_t* /*args*/) {
    C_0085ec18* provider = reinterpret_cast<C_0085ec18*>(self);

    void* providerObject = reinterpret_cast<void*>(provider->field_0004);
    void* referencedObject =
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, providerObject, 0)[0x10 / 4])(providerObject, nullptr);

    return hook::tail_jump(referencedObject, (*reinterpret_cast<void***>(referencedObject))[0x10 / 4]);
}

HOOK_TAIL(0x4F4EA0, ForwardProviderFrameState, "character_model");
