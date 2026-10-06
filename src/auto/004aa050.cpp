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

// Returns the literal name string "TIrAInteractivo".
static const char* __cdecl GetIrAInteractivoName()
{
    return reinterpret_cast<const char*>(0x83649c);
}

HOOK(0x4AA050, GetIrAInteractivoName, "auto");
