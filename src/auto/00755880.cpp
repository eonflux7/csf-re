#include "hook.h"

static int __cdecl GameConfig_IsResolvedEntryActive(int entry)
{
    return entry == 5 || entry == 6 || entry == 7 || entry == 4 ||
           entry == 19 || entry == 25 || entry == 30 || entry == 33;
}

HOOK(0x00755880, GameConfig_IsResolvedEntryActive, "auto");
