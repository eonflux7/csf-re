#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Setter for the byte field at offset 0x15 of this object.
// Stores the incoming byte and returns it unchanged (byte result in AL).
static char __fastcall SetByteField15(uint8_t* self, void* /*edx*/, char value)
{
    CSF_AT(char, self, 0x15) = value;
    return value;
}

HOOK(0x50AAA0, SetByteField15, "auto");
