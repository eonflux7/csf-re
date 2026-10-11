#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Target 0x006A80B0: reads the state word at +8. If the state is 2 or 0,
// writes the constant 0x494C3830 to the word at +0x2B0. Returns the state.
static int __fastcall NetGathering_ApplyMarkerWhenStateIdle(void* self, void* /*edx*/)
{
    int state = CSF_AT(int, self, 0x08);
    if (state == 2 || state == 0) {
        CSF_AT(int, self, 0x2B0) = 0x494C3830;
    }
    return state;
}

HOOK(0x006A80B0, NetGathering_ApplyMarkerWhenStateIdle, "auto");
