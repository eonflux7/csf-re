#include <cstdint>
#include "hook.h"
#include "globals.h"

// Tail-callee at 0x52B770: reads this in ecx, an extra value in ebx, and a float on the stack.
// ebx is a callee-saved register, so it still holds our caller's a2 at the call site and is
// forwarded unchanged, exactly like the original tail jump.
CSF_FUNC(int, __fastcall, CMenuMessageBox_UpdateTimersAndSelection, 0x52B770, (void* self, void* edx, float a3));

// Increments the phase counter at +0x9AC; while it stays below 3 the phase handler at
// 0x52B770 is forwarded to (tail jump). Once it reaches 3 the object's state at +0x28 is
// forced to 3 and the new counter value is returned.
static int __fastcall MenuMessageBox_IncrementPhaseCounter(void* self, void* /*edx*/, float a3)
{
    uint32_t counter = CSF_AT(uint32_t, self, 0x9AC) + 1;
    CSF_AT(uint32_t, self, 0x9AC) = counter;
    if (counter < 3u)
        return CMenuMessageBox_UpdateTimersAndSelection(self, nullptr, a3);
    CSF_AT(uint32_t, self, 0x28) = 3;
    return static_cast<int>(counter);
}

HOOK(0x52B930, MenuMessageBox_IncrementPhaseCounter, "auto");
