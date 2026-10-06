#include "globals.h"
#include "hook.h"

static const char* __cdecl GetGoToParapetTypeName()
{
    return CSF_STR(0x008361DC, "TIrAParapeto");
}

HOOK(0x004AA1B0, GetGoToParapetTypeName, "auto");
