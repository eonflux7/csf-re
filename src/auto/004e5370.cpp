#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Sets the result slot at out+4 to -1. If the request code is 14, dispatches
// through virtual slot 0xA8 with (a2, out). Returns the (possibly updated) result slot.
static int __fastcall C_0083e9d0_SetResultForCodeFourteen(C_0083e9d0* self, void* /*edx*/,
                                                          int a2, int a3, int a4) {
    uint8_t* outBlock = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a4));
    CSF_AT(int, outBlock, 4) = -1;
    if (a3 == 14) {
        self->vSlotA8(a2, a4);
    }
    return CSF_AT(int, outBlock, 4);
}

HOOK(0x4E5370, C_0083e9d0_SetResultForCodeFourteen, "auto");
