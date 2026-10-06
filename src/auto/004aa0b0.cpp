#include "globals.h"
#include "hook.h"
#include "legacy/string.h"

// Returns the name of the "TFncIrHacia" function type.
static const char* GetFncIrHaciaName()
{
    return CSF_STR(0x837374, "TFncIrHacia");
}

HOOK(0x4AA0B0, GetFncIrHaciaName, "auto");
