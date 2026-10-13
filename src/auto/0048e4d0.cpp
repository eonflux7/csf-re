#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x0048E4D0. Asks the object for its scene node (slot 0x28), asks that
// node for a payload object (slot 0x18), then tail-jumps into the payload's
// slot 0x34 handler with the caller's stack arguments untouched.
static hook::Tail __fastcall CPlayerCharacter_DispatchSceneObjectWithPayload(void* self, const uint32_t* args) {
    (void)args;
    auto* obj = static_cast<C_00833908*>(self);

    // slot 0x28 (vGetField016c) on this
    int sceneNodeValue = obj->vGetField016c();
    void* sceneNode = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(sceneNodeValue)));

    // slot 0x18 on the scene node (type unknown -> raw vtable access), __thiscall
    using PayloadGetter = int(__fastcall*)(void*, void*);
    auto getPayload = reinterpret_cast<PayloadGetter>(CSF_AT(void**, sceneNode, 0)[0x18 / 4]);
    int payloadValue = getPayload(sceneNode, nullptr);
    void* payload = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(payloadValue)));

    // Tail jump: ecx = payload, target = payload's vtable slot 0x34
    return hook::tail_jump(payload, (*reinterpret_cast<void***>(payload))[0x34 / 4]);
}

HOOK_TAIL(0x0048E4D0, CPlayerCharacter_DispatchSceneObjectWithPayload, "auto");
