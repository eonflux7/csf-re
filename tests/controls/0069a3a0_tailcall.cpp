#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x0069A3A0
// The original is a tail-jump thunk:
//   mov ecx, [ecx+4]        ; ecx = self->field_0004 (inner object)
//   mov eax, [ecx]          ; eax = inner object's vtable
//   jmp dword ptr [eax+8]   ; invoke vtable slot 2 (byte offset 8) with ecx = inner object
// It forwards the callee's return value (int) unchanged; no stack arguments are consumed.
typedef int(__fastcall* InnerObjectSlot2Fn)(void*, void*);

static int __fastcall ForwardToInnerObjectVirtualSlot2(C_0085e760* self, void* /*edx*/)
{
    void* innerObject = reinterpret_cast<void*>(self->field_0004);
    void** innerVtable = CSF_AT(void**, innerObject, 0);
    return reinterpret_cast<InnerObjectSlot2Fn>(innerVtable[2])(innerObject, nullptr);
}

HOOK(0x0069A3A0, ForwardToInnerObjectVirtualSlot2, "auto");
