#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the constant string "TSubir" (Spanish for "to climb / go up"),
// stored in the game's read-only data at 0x00834EC0.
static const char* __cdecl GetTSubirName_4A7D90()
{
    return reinterpret_cast<const char*>(CSF_ADDR_DATA(0x00834EC0));
}

HOOK(0x4A7D90, GetTSubirName_4A7D90, "auto");
