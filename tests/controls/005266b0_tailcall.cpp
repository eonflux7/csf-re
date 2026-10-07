#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x005266B0 (8 bytes, __thiscall, no stack args, no `ret N`):
//   mov ecx, [ecx + 0x40]   ; inner object pointer
//   mov eax, [ecx]          ; inner object's vtable
//   jmp [eax + 0x38]        ; tail-call virtual slot 0x38 (index 14), forwarding result
// The virtual is called with the inner object as `this` (ecx) and returns an int.
static int __fastcall InvokeField0040VirtualMethod(C_008410b0* self, void* /*edx*/)
{
    void* inner = reinterpret_cast<void*>(self->field_0040);
    return ((int(__fastcall*)(void*, void*))CSF_AT(void**, inner, 0)[0x38 / 4])(inner, nullptr);
}

HOOK(0x005266B0, InvokeField0040VirtualMethod, "auto");
