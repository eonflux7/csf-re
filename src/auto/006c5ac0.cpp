#include <cstdint>
#include "globals.h"
#include "hook.h"

// Compares two skin vertex records: float Y at +4 first, then a signed 16-bit key at +8.
// Returns -1 if a is greater on Y, 1 if less on Y, and on a Y tie orders by the 16-bit key
// (0 if equal, 1 if a's key is less, -1 otherwise).
static int __cdecl GfxSkin_CompareSkinKeys(float* a1, float* a2)
{
    if (a1[1] > a2[1])
        return -1;
    if (a1[1] < a2[1])
        return 1;

    int16_t keyA = CSF_AT(int16_t, a1, 0x8);
    int16_t keyB = CSF_AT(int16_t, a2, 0x8);
    if (keyA == keyB)
        return 0;
    return (keyA <= keyB) ? 1 : -1;
}

HOOK(0x6C5AC0, GfxSkin_CompareSkinKeys, "auto");
