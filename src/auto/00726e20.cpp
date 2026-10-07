#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Sets the active flag at +0x7C, clears the dword at +0x74, and returns the
// address of the embedded object located at +0x6C.
static int __fastcall C_008655e8_ResetFlagAndGetEmbedded(C_008655e8* self, void* /*edx*/)
{
    self->field_007c = 1;
    self->field_0074 = 0;
    return static_cast<int>(reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(self) + 0x6c));
}

HOOK(0x00726E20, C_008655e8_ResetFlagAndGetEmbedded, "auto");
