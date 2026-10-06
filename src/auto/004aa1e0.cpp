#include "globals.h"
#include "hook.h"

static const char* __cdecl GetParserTypeName()
{
    return CSF_STR(0x008381A4, "TParsear");
}

HOOK(0x004AA1E0, GetParserTypeName, "auto");
