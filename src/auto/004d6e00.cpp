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

// Returns the name of the "TPlayAnimacion" task/type string constant.
static const char* __cdecl GetTPlayAnimacionName()
{
    return CSF_STR(0x0083CB30, "TPlayAnimacion");
}

HOOK(0x004D6E00, GetTPlayAnimacionName, "auto");
