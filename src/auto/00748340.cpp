#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Returns the value stored for the given selector flag. The byte at +0x69 is
// the default, returned when the selected slot's flag is set. The slot is
// chosen by a2 == 1 (slot at +0x354, flag at +0x360) or any other value
// (slot at +0x35C, flag at +0x361).
static int __fastcall GetFlaggedValueForSlot(void* self, void* /*edx*/, char a2)
{
    int result = CSF_AT(signed char, self, 0x69);

    if (a2 == 1)
    {
        if (!CSF_AT(uint8_t, self, 0x360))
            return CSF_AT(int, self, 0x354);
    }
    else if (!CSF_AT(uint8_t, self, 0x361))
    {
        return CSF_AT(int, self, 0x35C);
    }

    return result;
}

HOOK(0x748340, GetFlaggedValueForSlot, "auto");
