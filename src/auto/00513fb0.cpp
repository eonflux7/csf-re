#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __cdecl ProfilesSavegames_CompareSaveGameEntries(float* left, float* right)
{
    const int32_t leftFirst = CSF_AT(int32_t, left, 0x1C);
    const int32_t leftSecond = CSF_AT(int32_t, left, 0x20);
    const int32_t leftThird = CSF_AT(int32_t, left, 0x24);
    const int32_t leftFourth = CSF_AT(int32_t, left, 0x28);
    const int32_t leftFifth = CSF_AT(int32_t, left, 0x2C);
    const float leftFraction = CSF_AT(float, left, 0x30);

    const int32_t rightFirst = CSF_AT(int32_t, right, 0x1C);
    const int32_t rightSecond = CSF_AT(int32_t, right, 0x20);
    const int32_t rightThird = CSF_AT(int32_t, right, 0x24);
    const int32_t rightFourth = CSF_AT(int32_t, right, 0x28);
    const int32_t rightFifth = CSF_AT(int32_t, right, 0x2C);
    const float rightFraction = CSF_AT(float, right, 0x30);

    if (leftFirst != rightFirst)
        return leftFirst > rightFirst ? -1 : 1;
    if (leftSecond != rightSecond)
        return leftSecond > rightSecond ? -1 : 1;
    if (leftThird != rightThird)
        return leftThird > rightThird ? -1 : 1;
    if (leftFourth != rightFourth)
        return leftFourth > rightFourth ? -1 : 1;
    if (leftFifth != rightFifth)
        return leftFifth > rightFifth ? -1 : 1;

    if (leftFraction == rightFraction)
    {
        const int32_t leftTieBreaker = CSF_AT(int32_t, left, 0xC4);
        const int32_t rightTieBreaker = CSF_AT(int32_t, right, 0xC4);
        if (leftTieBreaker < rightTieBreaker)
            return -1;
        return leftTieBreaker > rightTieBreaker;
    }

    return leftFraction > rightFraction ? -1 : 1;
}

HOOK(0x00513FB0, ProfilesSavegames_CompareSaveGameEntries, "auto");
