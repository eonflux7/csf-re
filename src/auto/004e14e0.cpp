#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original 0x4E19B0: __fastcall-style binding of a __thiscall callee taking one pointer-to-int argument.
CSF_FUNC(void, __fastcall, sub_4E19B0, 0x004E19B0, (void* self, void* edx, int* a2));

// Forwards this object's single int argument to 0x4E19B0 by address.
// The original copies the argument into its own stack slot and passes that slot's address,
// so the parameter a2 is passed by pointer here as well.
static void __fastcall C_0083e5e8_ForwardArgAddressToBase(C_0083e5e8* self, void* /*edx*/, int a2) {
    sub_4E19B0(self, nullptr, &a2);
}

HOOK(0x004E14E0, C_0083e5e8_ForwardArgAddressToBase, "auto");
