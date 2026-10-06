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

// Returns the localized token string "TMirar" (literally "T[o] Look/Mirar").
// Original: mov eax, 0x835794 ; ret  -> address of the string literal in .rdata.
static const char* __cdecl GetMirarToken()
{
    return "TMirar";
}

HOOK(0x4AA0C0, GetMirarToken, "auto");
