#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Tail-forwards to the virtual method at vtable slot 3 (offset 0xC) of the
// object stored at self->field_0004, passing that object as the `this` pointer.
// Original:
//   mov ecx, [ecx+4]   ; target = self->field_0004
//   mov eax, [ecx]     ; vtable = *target
//   jmp [eax+0xC]      ; tail-call vtable[3] with ecx = target
static int __fastcall C_0085e760_ForwardVirtualCallSlot3(C_0085e760* self, void* /*edx*/)
{
    void* target = reinterpret_cast<void*>(self->field_0004);
    void** target_vtbl = CSF_AT(void**, target, 0);
    return (reinterpret_cast<int(__fastcall*)(void*, void*)>(target_vtbl[3]))(target, nullptr);
}

HOOK(0x0069A3B0, C_0085e760_ForwardVirtualCallSlot3, "auto");
