#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Appends a typed character to the 0x3F-byte key buffer at this+0xC50.
// Ignores NUL, backspace (8), tab (9), carriage return (13) and escape (27).
// Stores into the first empty (zero) slot; does nothing if the buffer is full.
static void __fastcall CInputDirectInput_AppendTypedChar(CInputDirectInput* self, void* /*edx*/, char keyChar)
{
    if (keyChar == 0 || keyChar == 8 || keyChar == 9 || keyChar == 13 || keyChar == 27)
        return;

    uint32_t slotIndex = 0;
    while (CSF_AT(uint8_t, self, 0xC50 + slotIndex) != 0)
    {
        if (++slotIndex >= 0x3F)
            return;
    }
    CSF_AT(uint8_t, self, 0xC50 + slotIndex) = static_cast<uint8_t>(keyChar);
}

HOOK(0x713880, CInputDirectInput_AppendTypedChar, "auto");
