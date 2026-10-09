#include "hook.h"

// Returns 1 when the sound id lies in the inclusive signed range [69, 88], else 0.
static int __stdcall SoundManager_IsIdInVariationRange(int soundId) {
    return soundId >= 69 && soundId <= 88;
}

HOOK(0x525470, SoundManager_IsIdInVariationRange, "auto");
