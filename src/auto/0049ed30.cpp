#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns the address of the control-flags entry for index a2 in the
// 28-byte record array at field_0198: base + a2 * 28 + 24.
static int __fastcall C_008342e8_GetControlFlagsAddress(C_008342e8* self, void* /*edx*/, int a2) {
    return (int)(self->field_0198 + 28u * (uint32_t)a2 + 24u);
}

HOOK(0x0049ED30, C_008342e8_GetControlFlagsAddress, "auto");
