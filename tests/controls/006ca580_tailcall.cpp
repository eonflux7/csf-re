#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Forwards the call to the virtual method at vtable byte offset 0x54 (slot 21) of the
// object stored at offset +4 of this object, passing that inner object as `this`.
// The original is a tail-jump thunk (jmp [eax+0x54]); no stack arguments are involved.
static int __fastcall CallInnerVirtualSlot54(void* self, void* /*edx*/)
{
    void* inner = CSF_AT(void*, self, 0x04);
    void** inner_vtbl = *reinterpret_cast<void***>(inner);
    using Fn = int(__fastcall*)(void*, void*);
    return reinterpret_cast<Fn>(inner_vtbl[0x54 / 4])(inner, nullptr);
}

HOOK(0x006CA580, CallInnerVirtualSlot54, "auto");
