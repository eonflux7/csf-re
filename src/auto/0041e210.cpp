#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class initializer at 0x4DB3C0, reached by tail jump (ret 0, __thiscall, no stack args).
CSF_FUNC(void, __fastcall, CharTaskBase_Init_4DB3C0, 0x004DB3C0, (void* self, void* edx));

static int __fastcall C_0082e598_InstallVtablesAndInitBase(C_0082e598* self, void* edx)
{
    // Primary vtable pointer at offset 0 (not a declared field).
    CSF_AT(uint32_t, self, 0x0) = CSF_ADDR_VT(0x0082E598);
    // Secondary vtable pointer at offset 4 (not a declared field).
    CSF_AT(uint32_t, self, 0x4) = CSF_ADDR_VT(0x0082E57C);
    // Tail-call the base initializer, forwarding ecx/edx unchanged.
    // The original returns whatever eax holds after the callee; no value is defined here.
    CharTaskBase_Init_4DB3C0(self, edx);
    return 0;
}

HOOK(0x41E210, C_0082e598_InstallVtablesAndInitBase, "auto");
