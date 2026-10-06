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

// Returns the literal string identifier "TMoviendose".
// Original: mov eax, 0x82f190 ; ret   (0x82f190 -> "TMoviendose")
static const char* __cdecl Sub_420720()
{
    return "TMoviendose";
}

HOOK(0x420720, Sub_420720, "auto");
