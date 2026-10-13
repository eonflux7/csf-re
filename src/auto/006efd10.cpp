#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00860a30_SetupRenderTargetFromDesc, 0x006CA410, (C_00860a30* self, void* , int a2));

static int __fastcall C_00862240_SetupAndDelegateToSubobject(C_00862240* self, void* /*edx*/, int a2) {
    C_00860a30_SetupRenderTargetFromDesc(self, nullptr, a2);

    // The embedded sub-object at +0x24 is its own `this`; its first dword is its vtable.
    // Slot +0x18 => index 6.
    void** subobjectVtbl = reinterpret_cast<void**>(self->field_0024);
    return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(subobjectVtbl[6])(
        &self->field_0024, nullptr, a2);
}

HOOK(0x006EFD10, C_00862240_SetupAndDelegateToSubobject, "auto");
