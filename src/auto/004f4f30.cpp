#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x004F4F30 (__thiscall, ret 0). Chains two virtual calls, then tail-jumps.
static hook::Tail __fastcall C_0085ec18_GetProviderObject(void* self, const uint32_t* args) {
    (void)args;

    auto* fsm = static_cast<C_0085ec18*>(self);

    // ecx = this->field_0004; the object stored in the field is the receiver.
    void* providerOwner = reinterpret_cast<void*>(static_cast<uintptr_t>(fsm->field_0004));

    // call dword ptr [eax + 0x10] on providerOwner -> returns the referenced object.
    using GetReferencedObjectFn = void* (__fastcall*)(void* self, void* edx);
    void* referencedObject =
        reinterpret_cast<GetReferencedObjectFn>(CSF_AT(void**, providerOwner, 0)[0x10 / 4])(providerOwner, nullptr);

    // jmp dword ptr [edx + 0x14], where edx = vtable of referencedObject and ecx = referencedObject.
    return hook::tail_jump(referencedObject, (*reinterpret_cast<void***>(referencedObject))[0x14 / 4]);
}

HOOK_TAIL(0x004F4F30, C_0085ec18_GetProviderObject, "character_model");
