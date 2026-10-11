#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Destructor of the base class C_0083b958, called as a tail jump.
CSF_FUNC(int, __fastcall, C_0083b958_Destroy, 0x004D1140, (void* self, void* edx));

// Resets the vtables of C_0083b4a0 (and its embedded sub-object at +0x48) and tail-calls the base destructor.
static int __fastcall C_0083b4a0_Destroy(C_0083b4a0* self, void* /*edx*/) {
    CSF_AT(void*, self, 0x0) = CSF_VTBL(0x0083B4A0);
    CSF_AT(uint32_t, self, 0x4) = (uint32_t)(uintptr_t)CSF_VTBL(0x0083B484);
    CSF_AT(void*, self, 0x48) = CSF_VTBL(0x0082D410);
    return C_0083b958_Destroy(self, nullptr);
}

HOOK(0x004CF7F0, C_0083b4a0_Destroy, "auto");
