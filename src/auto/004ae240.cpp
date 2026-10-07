#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the name of the "movil a parapeto" (mobile-to-parapet) action.
static const char* __cdecl GetMovilAParapetoName()
{
    return CSF_STR(0x008393DC, "TMovilAParapeto");
}

HOOK(0x4AE240, GetMovilAParapetoName, "auto");
