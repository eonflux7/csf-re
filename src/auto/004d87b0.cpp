#include <cstdint>

#include "globals.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "hook.h"

// Clears bit 0 of the dword at [self->field_0008 + 0x18] and returns the address of that dword.
static uint32_t* __fastcall C_0083dae8_ClearInnerFlagBit0(C_0083dae8* self, void* /*edx*/)
{
    uint32_t* flags = &CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0008), 0x18);
    *flags &= 0xFFFFFFFEu;
    return flags;
}

HOOK(0x4D87B0, C_0083dae8_ClearInnerFlagBit0, "auto");
