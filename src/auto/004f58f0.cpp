#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x004F58F0 (__thiscall, ret 0). Fetches the object held in field_0004, calls its
// vtable slot 0x10 to get a referenced object, then tail-jumps into that object's vtable slot 0x50.
static hook::Tail __fastcall C_0085ec18_TailCallReferencedObjectSlot50(void* self, const uint32_t* args) {
    (void)args;

    auto* fsm = static_cast<C_0085ec18*>(self);

    // ecx = this->field_0004; that object is the receiver of the first virtual call.
    void* ownerObject = reinterpret_cast<void*>(static_cast<uintptr_t>(fsm->field_0004));

    // call dword ptr [eax + 0x10] on ownerObject -> returns the referenced object (v1).
    using GetReferencedObjectFn = void* (__fastcall*)(void* self, void* edx);
    void* referencedObject =
        reinterpret_cast<GetReferencedObjectFn>(CSF_AT(void**, ownerObject, 0)[0x10 / 4])(ownerObject, nullptr);

    // jmp dword ptr [edx + 0x50], where edx = vtable of referencedObject and ecx = referencedObject.
    return hook::tail_jump(referencedObject, (*reinterpret_cast<void***>(referencedObject))[0x50 / 4]);
}

HOOK_TAIL(0x004F58F0, C_0085ec18_TailCallReferencedObjectSlot50, "character_model");
