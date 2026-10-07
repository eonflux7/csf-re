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

// Returns the static name string "TFncEvitarOclusores".
// Original: mov eax, 0x83A774; ret
static const char* __cdecl GetTFncEvitarOclusoresName()
{
    return CSF_STR(0x83A774, "TFncEvitarOclusores");
}

HOOK(0x4B2970, GetTFncEvitarOclusoresName, "auto");
