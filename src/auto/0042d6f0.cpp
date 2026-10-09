#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());

// Resets the HUD widget's state block (zeroes the 0x04..0x2C and 0x40..0x5C
// fields, sets scale 1.0 at 0x30, creates an RW matrix at 0x38, and sets the
// flag byte at 0x3C). Returns the object pointer, as the original does in eax.
static int __fastcall Hud_ResetWidgetState(void* self, void* /*edx*/)
{
    CSF_AT(int, self, 0x04) = 0;
    CSF_AT(int, self, 0x08) = 0;
    CSF_AT(int, self, 0x0C) = 0;
    CSF_AT(int, self, 0x10) = 0;
    CSF_AT(int, self, 0x14) = 0;
    CSF_AT(int, self, 0x18) = 0;
    CSF_AT(int, self, 0x1C) = 0;
    CSF_AT(int, self, 0x20) = 0;
    CSF_AT(int, self, 0x24) = 0;
    CSF_AT(int, self, 0x28) = 0;
    CSF_AT(int, self, 0x2C) = 0;
    CSF_AT(std::uint32_t, self, 0x30) = 0x3F800000u;

    CSF_AT(std::uint32_t*, self, 0x38) = RwMatrixCreate();
    CSF_AT(std::uint8_t, self, 0x3C) = 1;

    CSF_AT(int, self, 0x40) = 0;
    CSF_AT(int, self, 0x44) = 0;
    CSF_AT(int, self, 0x48) = 0;
    CSF_AT(int, self, 0x4C) = 0;
    CSF_AT(int, self, 0x50) = 0;
    CSF_AT(int, self, 0x54) = 0;
    CSF_AT(int, self, 0x58) = 0;
    CSF_AT(int, self, 0x5C) = 0;

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(self));
}

HOOK(0x0042D6F0, Hud_ResetWidgetState, "auto");
