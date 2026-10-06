#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "legacy/string.h"

// Returns the constant tag string "TOrientarse" (address 0x835934 in the original image).
static const char* __cdecl GetOrientarseTag()
{
    return CSF_STR(0x835934, "TOrientarse");
}

HOOK(0x4AA160, GetOrientarseTag, "auto");
