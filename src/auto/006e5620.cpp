#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Sets or clears bit 0 of the byte at +0x02 of the sub-object pointed to by
// field_000c. Returns the byte value that was stored.
static char __fastcall C_00861ce0_SetSubObjectFlagBit0(C_00861ce0* self, void* /*edx*/, char enable)
{
    void* flagsObject = reinterpret_cast<void*>(self->field_000c);
    uint8_t flags = CSF_AT(uint8_t, flagsObject, 0x2);
    char result;
    if (enable)
        result = static_cast<char>(flags | 1);
    else
        result = static_cast<char>(flags & 0xFE);
    CSF_AT(char, flagsObject, 0x2) = result;
    return result;
}

HOOK(0x006E5620, C_00861ce0_SetSubObjectFlagBit0, "auto");
