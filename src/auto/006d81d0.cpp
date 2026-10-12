#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, C_00861188_ResetInheritedState, 0x006D7F40, (void* self, void* edx));

// Clears the active render object state: resets the inherited base state, then zeroes
// the eight per-object slots at +0xA4, +0xE8..+0xF4 and +0x130..+0x138. Returns 0.
static int __fastcall C_008611d4_ClearActiveRenderObject(C_008611d4* self, void* /*edx*/)
{
    C_00861188_ResetInheritedState(self, nullptr);
    CSF_AT(int, self, 0x00A4) = 0;
    CSF_AT(int, self, 0x00E8) = 0;
    CSF_AT(int, self, 0x00EC) = 0;
    CSF_AT(int, self, 0x00F0) = 0;
    CSF_AT(int, self, 0x00F4) = 0;
    CSF_AT(int, self, 0x0130) = 0;
    CSF_AT(int, self, 0x0134) = 0;
    CSF_AT(int, self, 0x0138) = 0;
    return 0;
}

HOOK(0x006D81D0, C_008611d4_ClearActiveRenderObject, "auto");
