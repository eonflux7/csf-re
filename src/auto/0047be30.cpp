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

// Returns the address of the constant string "TDisparar" (0x00832D58).
static const char* __fastcall TDisparar_GetTDispararString(TDisparar* /*self*/, void* /*edx*/)
{
    return CSF_STR(0x832D58, "TDisparar");
}

HOOK(0x47BE30, TDisparar_GetTDispararString, "auto");
