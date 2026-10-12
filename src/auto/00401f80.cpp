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

// Returns the length of a NUL-terminated string (not counting the terminator).
// Plain __cdecl function: never reads ecx, so no `this` parameter.
static unsigned int __cdecl MeasureCString(const char* a1) {
    return static_cast<unsigned int>(strlen(a1));
}

HOOK(0x401F80, MeasureCString, "auto");
