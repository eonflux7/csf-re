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

// Returns the constant string literal "TPrcEnfocadoPorEnemigo" stored at 0x00839AD8.
static const char* __cdecl GetPrcEnfocadoPorEnemigo()
{
  return CSF_STR(0x839AD8, "TPrcEnfocadoPorEnemigo");
}

HOOK(0x4AE4B0, GetPrcEnfocadoPorEnemigo, "auto");
