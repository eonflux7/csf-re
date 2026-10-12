#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Calls virtual slot 0x14 (vInvokeInnerVirtual34) with a2, then returns the
// result of virtual slot 0x18 (vForwardToInnerVirtualSlot13) called with a2.
static int __fastcall C_00860e68_InvokeInnerThenForward(C_00860e68* self, void* /*edx*/, int a2) {
    // Slot 0x14 / 4 = 5. Result is discarded by the original.
    ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[0x14 / 4])(self, nullptr, a2);

    // Slot 0x18 / 4 = 6. Its result is the return value.
    return ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[0x18 / 4])(self, nullptr, a2);
}

HOOK(0x006D0D30, C_00860e68_InvokeInnerThenForward, "auto");
