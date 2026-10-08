#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// CMenuControls state-machine helper (0x00542D80).
// If the control's state field (+0x2D8) is 1, move it to state 2 and flag the
// two companion fields (+0x2C8 = 1, +0x2CC = 2). Returns the resulting state
// (2 when the transition happened, 1 otherwise).
static int __fastcall CMenuControls_TryAdvanceToState2(void* self, void* /*edx*/, int /*a2*/)
{
    // The original loads the state only once, before any store.
    if (CSF_AT(int, self, 0x2d8) != 1)
        return 1;

    CSF_AT(int, self, 0x2c8) = 1;
    CSF_AT(int, self, 0x2cc) = 2;
    CSF_AT(int, self, 0x2d8) = 2;
    return 2;
}

HOOK(0x00542D80, CMenuControls_TryAdvanceToState2, "auto");
