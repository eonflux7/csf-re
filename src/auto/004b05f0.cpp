#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Callee bound by address; original is __fastcall-style with stack args (a2, &field).
CSF_FUNC(char, __fastcall, Static_DispatchWithFieldPointer, 0x004B0300,
         (void* self, void* edx, int a2, int a3));

// __thiscall(S_004b0200* this, int a2) ret 4.
// Original: pushes (this+4), pushes a2, calls 0x4B0300 with ecx=this and edx=a2
// (edx still holds a2 from the load of [ebp+8]). Returns the callee's result unchanged.
static char __fastcall S_004b0200_CallWithFieldAddress(S_004b0200* self, void* /*edx*/, int a2)
{
    return Static_DispatchWithFieldPointer(self,
                                           reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(a2))),
                                           a2,
                                           reinterpret_cast<int>(&self->field_0004));
}

HOOK(0x004B05F0, S_004b0200_CallWithFieldAddress, "auto");
