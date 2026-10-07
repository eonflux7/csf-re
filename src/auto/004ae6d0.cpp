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

// Returns the literal identifier "TMirarAlrededor" (look-around task/animation name).
static const char* __cdecl GetName_TMirarAlrededor()
{
    return CSF_STR(0x835BEC, "TMirarAlrededor");
}

HOOK(0x4AE6D0, GetName_TMirarAlrededor, "auto");
