#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class destructor at 0x4D1140, reached by tail jump (ret 0, __thiscall, no stack args).
CSF_FUNC(int, __fastcall, C_0083b958_Destroy, 0x004D1140, (C_0083b958* self, void* edx));

static int __fastcall C_0083c388_ResetVtablesThenDestroy(C_0083c388* self, void* edx)
{
    // Primary vtable pointer at offset 0 (not a declared field).
    CSF_AT(uint32_t, self, 0x0) = CSF_ADDR_VT(0x0083C388);
    // Data table pointer at offset 4 (not a declared field).
    CSF_AT(uint32_t, self, 0x4) = CSF_ADDR_DATA(0x0083C368);
    // Tail-call the base destructor, forwarding ecx/edx unchanged; its eax is our result.
    return C_0083b958_Destroy(reinterpret_cast<C_0083b958*>(self), edx);
}

HOOK(0x4D67A0, C_0083c388_ResetVtablesThenDestroy, "auto");
