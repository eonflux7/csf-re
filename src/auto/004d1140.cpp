#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base-class initializer of TCharTask, reached by tail jump.
CSF_FUNC(void, __fastcall, CharTaskBase_Init_4DB3C0, 0x004DB3C0, (void* self, void* edx));

// Destructor of C_0083b958 (derived from TCharTask). Installs its own vtable, the
// vtable of its embedded base sub-object at +0x4, and the vtable at +0x2C, then
// tail-calls the TCharTask base initializer.
static int __fastcall C_0083b958_Destroy(C_0083b958* self, void* /*edx*/) {
    CSF_AT(void*, self, 0x0) = CSF_VTBL(0x0083B958);
    CSF_AT(uint32_t, self, 0x4) = (uint32_t)(uintptr_t)CSF_VTBL(0x0083B938);
    CSF_AT(void*, self, 0x2C) = CSF_VTBL(0x0082D410);
    CharTaskBase_Init_4DB3C0(self, nullptr);
    return 0;
}

HOOK(0x004D1140, C_0083b958_Destroy, "auto");
