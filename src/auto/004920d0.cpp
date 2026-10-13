#include <cstdint>
#include "hook.h"

// Returns non-zero (1) when the action playback code is 5 or 6, otherwise 0.
static int __cdecl IsActionPlaybackFlagSet(int actionCode) {
    return (actionCode == 5 || actionCode == 6) ? 1 : 0;
}

HOOK(0x4920D0, IsActionPlaybackFlagSet, "auto");
