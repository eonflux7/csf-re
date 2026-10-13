#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "legacy/string.h"

CSF_FUNC(char*, __cdecl, FormatValueString, 0x0064A8F0, (void** a1, char* Format, ...));

static char __cdecl NetSpawnMessages_FormatCharacterValue(char* a1, int a2)
{
    FormatValueString(reinterpret_cast<void**>(a2), const_cast<char*>(CSF_STR(0x008680F4, "N/A")), static_cast<int>(*a1));
    return 1;
}

HOOK(0x0074B1E0, NetSpawnMessages_FormatCharacterValue, "auto");
