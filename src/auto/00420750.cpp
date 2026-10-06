#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the pointer to the game's "TMuerto" string constant.
static const char* __cdecl GetTMuertoString()
{
    return CSF_STR(0x82F3D8, "TMuerto");
}

HOOK(0x420750, GetTMuertoString, "auto");
