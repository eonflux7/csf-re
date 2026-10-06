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

// Returns the address of the literal string "TFncIrHacia" (0x837374).
// The original is a 6-byte stub: `mov eax, 0x837374 ; ret`.
static const char* __cdecl GetFncIrHaciaName()
{
    return "TFncIrHacia";
}

HOOK(0x4AA0B0, GetFncIrHaciaName, "auto");
