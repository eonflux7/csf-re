#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x004E26C0: stores a 32-bit value at this+0x44 and returns it unchanged.
// The class owning offset 0x44 is unknown, so the slot is accessed by offset.
static int __fastcall SetValueAt44(void* self, void* /*edx*/, int value)
{
    CSF_AT(uint32_t, self, 0x44) = static_cast<uint32_t>(value);
    return value;
}

HOOK(0x4E26C0, SetValueAt44, "auto");
