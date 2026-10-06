#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the constant action name string "TPreAction" (data at 0x0082EDD0).
static const char* GetTPreActionName()
{
    return CSF_STR(0x0082EDD0, "TPreAction");
}

HOOK(0x4206D0, GetTPreActionName, "auto");
