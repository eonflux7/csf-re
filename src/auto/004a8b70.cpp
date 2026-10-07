#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Sets the byte field at offset 0x38 and returns the stored value.
static char __fastcall C_0083b958_SetByte38(C_0083b958* self, void* /*edx*/, char value)
{
    CSF_AT(uint8_t, self, 0x38) = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x004A8B70, C_0083b958_SetByte38, "auto");
