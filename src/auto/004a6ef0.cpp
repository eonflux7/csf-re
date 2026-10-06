#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the pointer to the "TBajar" tag string constant.
static const char* __cdecl GetBajarTag()
{
    return CSF_STR(0x834D40, "TBajar");
}

HOOK(0x4A6EF0, GetBajarTag, "auto");
