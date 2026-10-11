#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Float constants in the game's data segment (+pi, +2*pi, -pi).
CSF_GLOBAL(float, g_HudPi, 0x82D748);
CSF_GLOBAL(float, g_HudTwoPi, 0x82D74C);
CSF_GLOBAL(float, g_HudNegativePi, 0x82D75C);

// Returns (a1 - a2) wrapped into [-pi, pi] (approximately), as a double.
// If a1 <= a2 the value is raised by 2*pi until it is >= -pi;
// otherwise it is lowered by 2*pi until it is <= pi.
static double __cdecl AngleDiffNormalized(float a1, float a2) {
    double result = a1 - a2;
    if (a1 <= a2) {
        while (result < g_HudNegativePi) {
            result = result + g_HudTwoPi;
        }
    } else {
        while (result > g_HudPi) {
            result = result - g_HudTwoPi;
        }
    }
    return result;
}

HOOK_KEEP_ECX(0x4068E0, AngleDiffNormalized, 2, "auto");
