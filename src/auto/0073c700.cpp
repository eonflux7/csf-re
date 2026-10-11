#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the value held at +0xD4. Always marks the byte at +0x14E as set.
// If the stored value equals the argument, the slot at +0xD4 is cleared.
static int __fastcall NetSession_ConsumeSlotIfMatches(void* self, void* /*edx*/, int expectedValue)
{
    int storedValue = CSF_AT(int, self, 0xD4);
    CSF_AT(uint8_t, self, 0x14E) = 1;
    if (storedValue == expectedValue)
    {
        CSF_AT(int, self, 0xD4) = 0;
    }
    return storedValue;
}

HOOK(0x0073C700, NetSession_ConsumeSlotIfMatches, "auto");
