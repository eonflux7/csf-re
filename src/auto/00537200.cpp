#include <cstdint>
#include "hook.h"
#include "core/memory.h"

// Returns the byte field at offset 0x182 of the object.
static char __fastcall GetField_0182(void* self, void* /*edx*/)
{
    return static_cast<char>(CSF_AT(uint8_t, self, 0x182));
}

HOOK(0x537200, GetField_0182, "auto");
