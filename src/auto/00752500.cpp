#include <cstdint>
#include "globals.h"
#include "hook.h"

static CSF_FUNC(char*, __cdecl, FormatValueString, 0x0064A8F0, (void** a1, char* Format, ...));

static char __cdecl NetSpawnMessages_FormatByteValue(char* value, int destination)
{
    FormatValueString(
        reinterpret_cast<void**>(destination),
        const_cast<char*>(CSF_STR(0x0083090C, "%d")),
        static_cast<int>(*reinterpret_cast<unsigned char*>(value)));
    return 1;
}

HOOK(0x00752500, NetSpawnMessages_FormatByteValue, "auto");
