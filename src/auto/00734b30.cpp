#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns the address of the string data held by this object.
// Small-string optimization: when the stored size (field_0038) is below 8 the
// characters live inline at +0x24, otherwise +0x24 holds the heap pointer.
static uint32_t* __fastcall C_008670f0_GetStringData(C_008670f0* self, void* /*edx*/)
{
    if (self->field_0038 < 8u)
        return reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(self) + 0x24);

    return reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(self->field_0024));
}

HOOK(0x734B30, C_008670f0_GetStringData, "auto");
