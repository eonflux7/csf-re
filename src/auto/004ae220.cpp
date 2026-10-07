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

// Returns the static string constant "TEstaticoParapeto" (0x00839594).
static const char* GetStaticParapetName()
{
    return CSF_STR(0x839594, "TEstaticoParapeto");
}

HOOK(0x4AE220, GetStaticParapetName, "auto");
