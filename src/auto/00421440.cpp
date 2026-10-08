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

// Trivial accessor: returns the localization key for the "free move" camera state.
// The original is a 6-byte leaf that just loads the address of the string literal
// stored in the game's read-only data (0x0082F850 -> "TFreeMove").
static const char* __fastcall TFreeMove_GetName(TFreeMove* /*self*/, void* /*edx*/)
{
    return reinterpret_cast<const char*>(CSF_ADDR_DATA(0x0082F850)); // "TFreeMove"
}

HOOK(0x421440, TFreeMove_GetName, "auto");
