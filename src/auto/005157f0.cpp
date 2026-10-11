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

// Compares up to `count` 16-bit wide characters of two buffers.
// Returns 0 if the first `count` characters are equal (or count is 0),
// -1 if the first differing character in `a` is less than the one in `b`,
// and 1 otherwise. Characters are compared as unsigned 16-bit values.
static int __cdecl ProfilesSavegames_CompareWideCharacters(uint16_t* a, uint16_t* b, int count)
{
    int remaining = count;
    if (remaining == 0)
        return 0;

    while (*a == *b)
    {
        ++a;
        ++b;
        if (--remaining == 0)
            return 0;
    }

    return *a < *b ? -1 : 1;
}

HOOK(0x005157F0, ProfilesSavegames_CompareWideCharacters, "auto");
