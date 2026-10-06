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

// Returns the constant tag string "TImpacto" (impact material / event tag).
// Original: mov eax, 0x82ea18 ; ret   (cdecl, returns pointer to the literal)
static const char* __cdecl GetImpactoTag()
{
    return "TImpacto";
}

HOOK(0x4206A0, GetImpactoTag, "auto");
