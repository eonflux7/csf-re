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

// Returns the string identifier "TOrientarse".
// Original: mov eax, 0x835934 ; ret   (pointer to a hardcoded Spanish string literal)
static const char* GetOrientarseTag()
{
    return "TOrientarse";
}

HOOK(0x4AA160, GetOrientarseTag, "auto");
