#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x0074F770: returns the static string "NetPostman" (0x008687C8).
static const char* __cdecl GetNetPostmanName()
{
    return CSF_STR(0x008687C8, "NetPostman");
}

HOOK(0x0074F770, GetNetPostmanName, "auto");
