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

// Returns the constant identifier string "TFncIrAParapeto".
static const char* __fastcall TFncIrAParapeto_GetIdentifier(TFncIrAParapeto* self, void* /*edx*/)
{
    (void)self;
    return CSF_STR(0x00839DF4, "TFncIrAParapeto");
}

HOOK(0x004AE570, TFncIrAParapeto_GetIdentifier, "auto");
